//
// Created by yhkim on 25. 6. 5.
//

#include "rtc_sender/observers/peer_connection_observer.h"

#include "rtc_sender/logger/log.h"
#include "rtc_sender/gcs_connector.h"
#include "rtc_sender/signaling/signaling_client.h"

using namespace rtc_sender::observers;

void PeerConnectionObserver::OnDataChannel(webrtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) {
    gcs_connector_->OnDataChannel(data_channel);
}

void PeerConnectionObserver::OnIceCandidate(const webrtc::IceCandidateInterface *candidate) {
    if (!candidate) {
        RTC_SENDER_LOG_DEBUG("Received null ICE candidate");
        return;
    }
    RTC_SENDER_LOG_DEBUG("Gathered candidate: {} {}", candidate->sdp_mid(), candidate->sdp_mline_index());

    pc_manager_->SendIceCandidate(candidate);
}

void PeerConnectionObserver::OnStandardizedIceConnectionChange(
    webrtc::PeerConnectionInterface::IceConnectionState new_state) {
    RTC_SENDER_LOG_INFO("PeerConnectionObserver::OnStandardizedIceConnectionChange: {}",
                        webrtc::PeerConnectionInterface::AsString(new_state));
}

void PeerConnectionObserver::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) {
    if (new_state == webrtc::PeerConnectionInterface::IceGatheringState::kIceGatheringComplete) {
        RTC_SENDER_LOG_INFO("ICE Gathering Complete");
    }
}

void PeerConnectionObserver::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) {
    RTC_SENDER_LOG_INFO("PeerConnectionObserver::OnSignalingChange: {}",
                        webrtc::PeerConnectionInterface::AsString(new_state));
}

void PeerConnectionObserver::OnConnectionChange(webrtc::PeerConnectionInterface::PeerConnectionState new_state) {
    switch (new_state) {
        case webrtc::PeerConnectionInterface::PeerConnectionState::kNew:
            RTC_SENDER_LOG_INFO("PeerConnectionObserver::OnConnectionChange: New");
            break;
        case webrtc::PeerConnectionInterface::PeerConnectionState::kConnecting:
            RTC_SENDER_LOG_INFO("PeerConnectionObserver::OnConnectionChange: Connecting");
            break;
        case webrtc::PeerConnectionInterface::PeerConnectionState::kConnected:
            RTC_SENDER_LOG_INFO("PeerConnectionObserver::OnConnectionChange: Connected");
            pc_manager_->OnConnectionConnected();
            break;
        case webrtc::PeerConnectionInterface::PeerConnectionState::kFailed:
            RTC_SENDER_LOG_INFO("PeerConnectionObserver::OnConnectionChange: Failed");
            pc_manager_->ClosePeerConnection();
            break;
        case webrtc::PeerConnectionInterface::PeerConnectionState::kDisconnected:
        case webrtc::PeerConnectionInterface::PeerConnectionState::kClosed:
            RTC_SENDER_LOG_INFO("PeerConnectionObserver::OnConnectionChange: Disconnected");
            pc_manager_->ClosePeerConnection();
            break;
    }
}
