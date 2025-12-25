//
// Created by yhkim on 25. 6. 3.
//

#include <rtc_sender/client_state_manager.h>
#include <rtc_sender/handlers/data_channel/i_data_channel_receivable.h>
#include <rtc_sender/logger/log.h>
#include <rtc_sender/observers/peer_connection_observer.h>
#include <rtc_sender/gcs_connector.h>
#include <rtc_sender/signaling/signaling_client.h>

#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/video_codecs/video_decoder_factory_template.h"

class rtc_sender::GCSConnector::Impl {
public:
    explicit Impl(const std::string &robot_id,
                  const std::string &user_id,
                  const std::shared_ptr<ClientStateManager> &state_manager)
        : state_manager_(state_manager), robot_id_(robot_id), user_id_(user_id) {
    }

    std::string GetRobotId() const {
        return robot_id_;
    }

    std::string GetUserId() const {
        return user_id_;
    }

    void SetPeerConnectionManager(const std::shared_ptr<PeerConnectionManager> &peer_connection_manager) {
        peer_connection_manager_ = peer_connection_manager;
    }

    void InitializeWebRTC() const {
        peer_connection_manager_->InitializeWebRTC();
    }

    void AddDataChannelHandler(const std::shared_ptr<handlers::ADataChannelHandler> &data_channel_handler) {
        std::string label = data_channel_handler->GetLabel();
        if (data_channel_handlers_dict_.find(label) != data_channel_handlers_dict_.end()) {
            // If the label already exists, we can either throw an error or log a warning.
            RTC_SENDER_LOG_ERROR("Data channel handler with label '{}' already exists.", label);
            // TODO
            return;
        }

        // Add the data channel handler to the dictionary.
        data_channel_handlers_dict_.insert(std::pair(label, data_channel_handler));
    }

    void AddMediaTrackHandler(const std::shared_ptr<handlers::IMediaTrackHandler> &media_track_handler) {
        std::string track_name = media_track_handler->GetTrackName();
        if (media_track_handlers_dict_.find(track_name) != media_track_handlers_dict_.end()) {
            // If the track name already exists, we can either throw an error or log a warning.
            RTC_SENDER_LOG_ERROR("Media track handler with track name '{}' already exists.", track_name);
            // TODO
            return;
        }

        // Add the media track handler to the dictionary.
        media_track_handlers_dict_.insert(std::pair(track_name, media_track_handler));
    }

    void HandleSdpOffer(webrtc::SessionDescriptionInterface *sdp_offer) const {
        peer_connection_manager_->HandleSdpOffer(sdp_offer);
    }

    void HandleIceCandidateFromSignaling(webrtc::IceCandidateInterface *candidate) const {
        peer_connection_manager_->HandleIceCandidateFromSignaling(candidate);
    }

    void ClosePeerConnection() const {
        peer_connection_manager_->ClosePeerConnection();
    }

    void ShuttingDown() const {
        state_manager_->SetState(ClientStateManager::SHUTTING_DOWN);
        RTC_SENDER_LOG_INFO("Shutting down RobotWebRTCClient...");
        peer_connection_manager_->StopSignalingServer();
        peer_connection_manager_->ClosePeerConnection();
        peer_connection_manager_->DestroyPeerConnectionManager();
        RTC_SENDER_LOG_INFO("RobotWebRTCClient shutdown complete.");
    }

    std::shared_ptr<PeerConnectionManager> peer_connection_manager_ = nullptr;
    std::shared_ptr<ClientStateManager> state_manager_;

private:
    std::string robot_id_;
    std::string user_id_;

    // 데이터 채널 핸들러를 label 과 함께 저장합니다.
    std::map<std::string, std::shared_ptr<handlers::ADataChannelHandler> > data_channel_handlers_dict_ = {};
    // 미디어 트랙 핸들러를 track name 과 함께 저장합니다.
    std::map<std::string, std::shared_ptr<handlers::IMediaTrackHandler> > media_track_handlers_dict_ = {};

    // called by PeerConnectionObserver
    void OnDataChannel(webrtc::scoped_refptr<webrtc::DataChannelInterface> &data_channel) {
        std::string label = data_channel->label();
        RTC_SENDER_LOG_INFO("Data channel created with label: {}", label);
        // Check if the label exists in the data channel handlers dictionary
        auto it = data_channel_handlers_dict_.find(label);
        if (it == data_channel_handlers_dict_.end()) {
            // If it does exists, return
            return;
        }
        const std::shared_ptr<handlers::ADataChannelHandler> data_channel_handler = it->second;

        if (data_channel_handler->GetState() == handlers::ADataChannelHandler::kOpen) {
            RTC_SENDER_LOG_INFO("Data channel with label '{}' is already open.", label);
            return;
        }

        data_channel_handler->SetDataChannelInterface(data_channel);
        data_channel_handler->RegisterDataChannelObserver(data_channel_handler.get());
        data_channel_handler->AfterCreate();
    }

    // called by PeerConnectionManager
    void CreateAllMediaTracks() {
        // 미디어 트랙을 추가합니다.
        for (const auto &[track_name, media_track_handler]: media_track_handlers_dict_) {
            // Create video track source
            auto video_source = media_track_handler->CreateVideoTrackSource();
            if (!video_source) {
                RTC_SENDER_LOG_ERROR("Failed to create video track source for {}", track_name);
                continue;
            }

            // Create video track
            auto video_track = peer_connection_manager_->CreateVideoTrack(video_source, track_name);
            if (!video_track) {
                RTC_SENDER_LOG_ERROR("Failed to create video track for {}", track_name);
                continue;
            }

            // Add the video track to the peer connection
            auto add_track_result = peer_connection_manager_->AddTrack(video_track, {track_name});
            if (!add_track_result) {
                RTC_SENDER_LOG_ERROR("Failed to add video track for {}", track_name);
            }
        }
    }

    // called by PeerConnectionManager
    void CloseAllMediaTracks() {
        for (const auto &[track_name, media_track_handler]: media_track_handlers_dict_) {
            media_track_handler->Close();
        }
    }

    // called by PeerConnectionManager
    void CloseAllDataChannels() {
        for (const auto &[label, channel]: data_channel_handlers_dict_) {
            channel->CloseDataChannel();
        }
    }

    friend class GCSConnector;
};

rtc_sender::GCSConnector::GCSConnector(const std::string &robot_id,
                                       const std::string &user_id,
                                       const std::shared_ptr<ClientStateManager> &state_manager)
    : pImpl(std::make_unique<Impl>(robot_id, user_id, state_manager)) {
}

std::string rtc_sender::GCSConnector::GetRobotId() const {
    return pImpl->GetRobotId();
}

std::string rtc_sender::GCSConnector::GetUserId() const {
    return pImpl->GetUserId();
}

void rtc_sender::GCSConnector::SetPeerConnectionManager(
    const std::shared_ptr<PeerConnectionManager> &peer_connection_manager) const {
    pImpl->SetPeerConnectionManager(peer_connection_manager);
}

void rtc_sender::GCSConnector::InitializeWebRTC() const {
    pImpl->InitializeWebRTC();
}

void rtc_sender::GCSConnector::AddDataChannelHandler(
    const std::shared_ptr<handlers::ADataChannelHandler> &data_channel_handler) const {
    pImpl->AddDataChannelHandler(data_channel_handler);
}

void rtc_sender::GCSConnector::AddMediaTrackHandler(
    const std::shared_ptr<handlers::IMediaTrackHandler> &media_track_handler) const {
    pImpl->AddMediaTrackHandler(media_track_handler);
}

void rtc_sender::GCSConnector::HandleSdpOffer(webrtc::SessionDescriptionInterface *sdp_offer) const {
    pImpl->HandleSdpOffer(sdp_offer);
}

void rtc_sender::GCSConnector::HandleIceCandidateFromSignaling(webrtc::IceCandidateInterface *candidate) const {
    pImpl->HandleIceCandidateFromSignaling(candidate);
}

void rtc_sender::GCSConnector::ClosePeerConnection() const {
    pImpl->ClosePeerConnection();
}

void rtc_sender::GCSConnector::ShuttingDown() const {
    pImpl->ShuttingDown();
}

rtc_sender::GCSConnector::~GCSConnector() {
    ShuttingDown();
}

void rtc_sender::GCSConnector::CreateAllMediaTracks() const {
    pImpl->CreateAllMediaTracks();
}

void rtc_sender::GCSConnector::CloseAllMediaTracks() const {
    pImpl->CloseAllMediaTracks();
}

void rtc_sender::GCSConnector::CloseAllDataChannels() const {
    pImpl->CloseAllDataChannels();
}

void rtc_sender::GCSConnector::OnDataChannel(
    webrtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) const {
    pImpl->OnDataChannel(data_channel);
}

std::shared_ptr<rtc_sender::ClientStateManager> rtc_sender::GCSConnector::GetStateManager() const {
    return pImpl->state_manager_;
}
