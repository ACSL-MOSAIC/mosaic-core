//
// Created by yhkim on 25. 6. 5.
//

#include <mosaic_rtc_core/core/mosaic_connector.h>
#include <mosaic_rtc_core/logger/log.h>
#include <mosaic_rtc_core/observers/peer_connection_observer.h>

using namespace mosaic::core_observers;

void PeerConnectionObserver::OnDataChannel(const webrtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) {
    gcs_connector_->OnDataChannel(data_channel);
}

void PeerConnectionObserver::OnIceCandidate(const webrtc::IceCandidateInterface* candidate) {
    if (!candidate) {
        MOSAIC_LOG_DEBUG("Received null ICE candidate");
        return;
    }
    MOSAIC_LOG_DEBUG("Gathered candidate: {} {}", candidate->sdp_mid(), candidate->sdp_mline_index());

    pc_manager_->SendIceCandidate(candidate);
}

void PeerConnectionObserver::OnStandardizedIceConnectionChange(
    const webrtc::PeerConnectionInterface::IceConnectionState new_state) {
    MOSAIC_LOG_INFO("PeerConnectionObserver::OnStandardizedIceConnectionChange: {}",
                    webrtc::PeerConnectionInterface::AsString(new_state));
}

void PeerConnectionObserver::OnIceGatheringChange(const webrtc::PeerConnectionInterface::IceGatheringState new_state) {
    if (new_state == webrtc::PeerConnectionInterface::IceGatheringState::kIceGatheringComplete) {
        MOSAIC_LOG_INFO("ICE Gathering Complete");
    }
}

void PeerConnectionObserver::OnSignalingChange(const webrtc::PeerConnectionInterface::SignalingState new_state) {
    MOSAIC_LOG_INFO("PeerConnectionObserver::OnSignalingChange: {}",
                    webrtc::PeerConnectionInterface::AsString(new_state));
}

void PeerConnectionObserver::OnConnectionChange(const webrtc::PeerConnectionInterface::PeerConnectionState new_state) {
    switch (new_state) {
        case webrtc::PeerConnectionInterface::PeerConnectionState::kNew:
            MOSAIC_LOG_INFO("PeerConnectionObserver::OnConnectionChange: New");
            break;
        case webrtc::PeerConnectionInterface::PeerConnectionState::kConnecting:
            MOSAIC_LOG_INFO("PeerConnectionObserver::OnConnectionChange: Connecting");
            break;
        case webrtc::PeerConnectionInterface::PeerConnectionState::kConnected:
            MOSAIC_LOG_INFO("PeerConnectionObserver::OnConnectionChange: Connected");
            pc_manager_->OnConnectionConnected();
            break;
        case webrtc::PeerConnectionInterface::PeerConnectionState::kFailed:
            MOSAIC_LOG_INFO("PeerConnectionObserver::OnConnectionChange: Failed");
            pc_manager_->ClosePeerConnection();
            break;
        case webrtc::PeerConnectionInterface::PeerConnectionState::kDisconnected:
        case webrtc::PeerConnectionInterface::PeerConnectionState::kClosed:
            MOSAIC_LOG_INFO("PeerConnectionObserver::OnConnectionChange: Disconnected");
            pc_manager_->ClosePeerConnection();
            break;
    }
}
