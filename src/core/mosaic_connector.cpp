//
// Created by yhkim on 25. 6. 3.
//

#include <utility>

#include <api/audio_codecs/builtin_audio_decoder_factory.h>
#include <api/video_codecs/video_decoder_factory_template.h>
#include <mosaic_rtc_core/core/mosaic_connector.h>
#include <mosaic_rtc_core/logger/log.h>
#include <mosaic_rtc_core/observers/peer_connection_observer.h>

using namespace mosaic::core;

class MosaicConnector::Impl {
  public:
    explicit Impl(std::string robot_id,
                  std::string user_id,
                  const std::shared_ptr<ConnectorStateManager>& state_manager)
        : state_manager_(state_manager), robot_id_(std::move(robot_id)), user_id_(std::move(user_id)) {}

    [[nodiscard]] std::string GetRobotId() const {
        return robot_id_;
    }

    [[nodiscard]] std::string GetUserId() const {
        return user_id_;
    }

    void SetPeerConnectionManager(const std::shared_ptr<PeerConnectionManager>& peer_connection_manager) {
        peer_connection_manager_ = peer_connection_manager;
    }

    void InitializeWebRTC() const {
        peer_connection_manager_->InitializeWebRTC();
    }

    void AddDataChannelHandler(const std::shared_ptr<handlers::IDataChannelHandler>& data_channel_handler) {
        std::string label = data_channel_handler->GetLabel();
        if (data_channel_handlers_dict_.find(label) != data_channel_handlers_dict_.end()) {
            // If the label already exists, we can either throw an error or log a warning.
            MOSAIC_LOG_ERROR("Data channel handler with label '{}' already exists.", label);
            // TODO
            return;
        }

        // Add the data channel handler to the dictionary.
        data_channel_handlers_dict_.insert(std::pair(label, data_channel_handler));
    }

    void AddMediaTrackHandler(const std::shared_ptr<handlers::IMediaTrackHandler>& media_track_handler) {
        std::string track_name = media_track_handler->GetTrackName();
        if (media_track_handlers_dict_.find(track_name) != media_track_handlers_dict_.end()) {
            // If the track name already exists, we can either throw an error or log a warning.
            MOSAIC_LOG_ERROR("Media track handler with track name '{}' already exists.", track_name);
            // TODO
            return;
        }

        // Add the media track handler to the dictionary.
        media_track_handlers_dict_.insert(std::pair(track_name, media_track_handler));
    }

    void HandleSdpOffer(webrtc::SessionDescriptionInterface* sdp_offer) const {
        peer_connection_manager_->HandleSdpOffer(sdp_offer);
    }

    void HandleIceCandidateFromSignaling(const webrtc::IceCandidateInterface* candidate) const {
        peer_connection_manager_->HandleIceCandidateFromSignaling(candidate);
    }

    void ClosePeerConnection() const {
        peer_connection_manager_->ClosePeerConnection();
    }

    void ShuttingDown() const {
        state_manager_->SetState(ConnectorStateManager::SHUTTING_DOWN);
        MOSAIC_LOG_INFO("Shutting down RobotWebRTCClient...");
        peer_connection_manager_->StopSignalingServer();
        peer_connection_manager_->ClosePeerConnection();
        peer_connection_manager_->DestroyPeerConnectionManager();
        MOSAIC_LOG_INFO("RobotWebRTCClient shutdown complete.");
    }

    std::shared_ptr<PeerConnectionManager> peer_connection_manager_ = nullptr;
    std::shared_ptr<ConnectorStateManager> state_manager_;

  private:
    std::string robot_id_;
    std::string user_id_;

    // 데이터 채널 핸들러를 label 과 함께 저장합니다.
    std::map<std::string, std::shared_ptr<handlers::IDataChannelHandler>> data_channel_handlers_dict_ = {};
    // 미디어 트랙 핸들러를 track name 과 함께 저장합니다.
    std::map<std::string, std::shared_ptr<handlers::IMediaTrackHandler>> media_track_handlers_dict_ = {};

    // called by PeerConnectionObserver
    void OnDataChannel(const webrtc::scoped_refptr<webrtc::DataChannelInterface>& data_channel) {
        std::string label = data_channel->label();
        MOSAIC_LOG_INFO("Data channel created with label: {}", label);
        // Check if the label exists in the data channel handlers dictionary
        const auto it = data_channel_handlers_dict_.find(label);
        if (it == data_channel_handlers_dict_.end()) {
            // If it does exists, return
            return;
        }
        const std::shared_ptr<handlers::IDataChannelHandler> data_channel_handler = it->second;

        if (data_channel_handler->GetState() == handlers::IDataChannelHandler::kOpen) {
            MOSAIC_LOG_INFO("Data channel with label '{}' is already open.", label);
            return;
        }

        data_channel_handler->SetDataChannelInterface(data_channel);
        data_channel_handler->RegisterDataChannelObserver(data_channel_handler.get());
        data_channel_handler->AfterCreate();
    }

    // called by PeerConnectionManager
    void CreateAllMediaTracks() {
        // 미디어 트랙을 추가합니다.
        for (const auto& [track_name, media_track_handler] : media_track_handlers_dict_) {
            // Create video track source
            auto video_source = media_track_handler->CreateVideoTrackSource();
            if (!video_source) {
                MOSAIC_LOG_ERROR("Failed to create video track source for {}", track_name);
                continue;
            }

            // Create video track
            auto video_track = peer_connection_manager_->CreateVideoTrack(video_source, track_name);
            if (!video_track) {
                MOSAIC_LOG_ERROR("Failed to create video track for {}", track_name);
                continue;
            }

            // Add the video track to the peer connection
            auto add_track_result = peer_connection_manager_->AddTrack(video_track, {track_name});
            if (!add_track_result) {
                MOSAIC_LOG_ERROR("Failed to add video track for {}", track_name);
            }
        }
    }

    // called by PeerConnectionManager
    void CloseAllMediaTracks() {
        for (const auto& [track_name, media_track_handler] : media_track_handlers_dict_) {
            media_track_handler->Close();
        }
    }

    // called by PeerConnectionManager
    void CloseAllDataChannels() {
        for (const auto& [label, channel] : data_channel_handlers_dict_) {
            channel->CloseDataChannel();
        }
    }

    friend class MosaicConnector;
};

MosaicConnector::MosaicConnector(const std::string& robot_id,
                                 const std::string& user_id,
                                 const std::shared_ptr<ConnectorStateManager>& state_manager)
    : pImpl(std::make_unique<Impl>(robot_id, user_id, state_manager)) {}

std::string MosaicConnector::GetRobotId() const {
    return pImpl->GetRobotId();
}

std::string MosaicConnector::GetUserId() const {
    return pImpl->GetUserId();
}

void MosaicConnector::SetPeerConnectionManager(
    const std::shared_ptr<PeerConnectionManager>& peer_connection_manager) const {
    pImpl->SetPeerConnectionManager(peer_connection_manager);
}

void MosaicConnector::InitializeWebRTC() const {
    pImpl->InitializeWebRTC();
}

void MosaicConnector::AddDataChannelHandler(
    const std::shared_ptr<handlers::IDataChannelHandler>& data_channel_handler) const {
    pImpl->AddDataChannelHandler(data_channel_handler);
}

void MosaicConnector::AddMediaTrackHandler(
    const std::shared_ptr<handlers::IMediaTrackHandler>& media_track_handler) const {
    pImpl->AddMediaTrackHandler(media_track_handler);
}

void MosaicConnector::HandleSdpOffer(webrtc::SessionDescriptionInterface* sdp_offer) const {
    pImpl->HandleSdpOffer(sdp_offer);
}

void MosaicConnector::HandleIceCandidateFromSignaling(webrtc::IceCandidateInterface* candidate) const {
    pImpl->HandleIceCandidateFromSignaling(candidate);
}

void MosaicConnector::ClosePeerConnection() const {
    pImpl->ClosePeerConnection();
}

void MosaicConnector::ShuttingDown() const {
    pImpl->ShuttingDown();
}

MosaicConnector::~MosaicConnector() {
    ShuttingDown();
}

void MosaicConnector::CreateAllMediaTracks() const {
    pImpl->CreateAllMediaTracks();
}

void MosaicConnector::CloseAllMediaTracks() const {
    pImpl->CloseAllMediaTracks();
}

void MosaicConnector::CloseAllDataChannels() const {
    pImpl->CloseAllDataChannels();
}

void MosaicConnector::OnDataChannel(const webrtc::scoped_refptr<webrtc::DataChannelInterface>& data_channel) const {
    pImpl->OnDataChannel(data_channel);
}

std::shared_ptr<ConnectorStateManager> MosaicConnector::GetStateManager() const {
    return pImpl->state_manager_;
}
