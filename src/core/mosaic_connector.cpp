//
// Created by yhkim on 25. 6. 3.
//

#include <utility>

#include <api/audio_codecs/builtin_audio_decoder_factory.h>
#include <api/video_codecs/video_decoder_factory_template.h>
#include <mosaic/core/mosaic_connector.h>
#include <mosaic/core/peer_connection_observer.h>
#include <mosaic/logger/log.h>

using namespace mosaic::core;

MosaicConnector::MosaicConnector(const std::shared_ptr<ConnectorStateManager>& state_manager)
    : state_manager_(state_manager) {}

MosaicConnector::~MosaicConnector() {
    ShuttingDown();
}

void MosaicConnector::SetPeerConnectionManager(
    const std::shared_ptr<PeerConnectionManager>& peer_connection_manager) const {
    const_cast<MosaicConnector*>(this)->peer_connection_manager_ = peer_connection_manager;
}

void MosaicConnector::InitializeWebRTC() const {
    peer_connection_manager_->InitializeWebRTC();
}

void MosaicConnector::AddDataChannelHandler(
    const std::shared_ptr<handlers::IDataChannelHandler>& data_channel_handler) const {
    std::string label = data_channel_handler->GetLabel();
    if (data_channel_handlers_dict_.find(label) != data_channel_handlers_dict_.end()) {
        MOSAIC_LOG_ERROR("Data channel handler with label '{}' already exists.", label);
        return;
    }
    const_cast<MosaicConnector*>(this)->data_channel_handlers_dict_.insert(std::pair(label, data_channel_handler));
}

void MosaicConnector::AddMediaTrackHandler(
    const std::shared_ptr<handlers::IMediaTrackHandler>& media_track_handler) const {
    std::string track_name = media_track_handler->GetTrackName();
    if (media_track_handlers_dict_.find(track_name) != media_track_handlers_dict_.end()) {
        MOSAIC_LOG_ERROR("Media track handler with track name '{}' already exists.", track_name);
        return;
    }
    const_cast<MosaicConnector*>(this)->media_track_handlers_dict_.insert(std::pair(track_name, media_track_handler));
}

void MosaicConnector::HandleSdpOffer(webrtc::SessionDescriptionInterface* sdp_offer) const {
    peer_connection_manager_->HandleSdpOffer(sdp_offer);
}

void MosaicConnector::HandleIceCandidateFromSignaling(webrtc::IceCandidateInterface* candidate) const {
    peer_connection_manager_->HandleIceCandidateFromSignaling(candidate);
}

void MosaicConnector::ClosePeerConnection() const {
    peer_connection_manager_->ClosePeerConnection();
}

void MosaicConnector::ShuttingDown() const {
    state_manager_->SetState(ConnectorStateManager::DISCONNECTED);
    MOSAIC_LOG_INFO("Shutting down RobotWebRTCClient...");
    peer_connection_manager_->StopSignalingServer();
    peer_connection_manager_->ClosePeerConnection();
    peer_connection_manager_->DestroyPeerConnectionManager();
    MOSAIC_LOG_INFO("RobotWebRTCClient shutdown complete.");
}

void MosaicConnector::OnDataChannel(const webrtc::scoped_refptr<webrtc::DataChannelInterface>& data_channel) const {
    std::string label = data_channel->label();
    MOSAIC_LOG_INFO("Data channel created with label: {}", label);

    const auto it = data_channel_handlers_dict_.find(label);
    if (it == data_channel_handlers_dict_.end()) {
        return;
    }
    const std::shared_ptr<handlers::IDataChannelHandler> data_channel_handler = it->second;

    if (data_channel_handler->GetState() == handlers::IDataChannelHandler::kOpen) {
        MOSAIC_LOG_INFO("Data channel with label '{}' is already open.", label);
        return;
    }

    data_channel_handler->SetDataChannelInterface(data_channel);
    data_channel_handler->RegisterDataChannelObserver();
    data_channel_handler->AfterCreate();
}

void MosaicConnector::CreateAllMediaTracks() const {
    for (const auto& [track_name, media_track_handler] : media_track_handlers_dict_) {
        auto video_source = media_track_handler->CreateVideoTrackSource();
        if (!video_source) {
            MOSAIC_LOG_ERROR("Failed to create video track source for {}", track_name);
            continue;
        }

        auto video_track = peer_connection_manager_->CreateVideoTrack(video_source, track_name);
        if (!video_track) {
            MOSAIC_LOG_ERROR("Failed to create video track for {}", track_name);
            continue;
        }

        auto add_track_result = peer_connection_manager_->AddTrack(video_track, {track_name});
        if (!add_track_result) {
            MOSAIC_LOG_ERROR("Failed to add video track for {}", track_name);
        }
    }
}

void MosaicConnector::CloseAllMediaTracks() const {
    for (const auto& [track_name, media_track_handler] : media_track_handlers_dict_) {
        media_track_handler->Close();
    }
}

void MosaicConnector::CloseAllDataChannels() const {
    for (const auto& [label, channel] : data_channel_handlers_dict_) {
        channel->CloseDataChannel();
    }
}

std::shared_ptr<ConnectorStateManager> MosaicConnector::GetStateManager() const {
    return state_manager_;
}
