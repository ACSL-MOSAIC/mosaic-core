//
// Created by yhkim on 25. 6. 5.
//

#ifndef PEER_CONNECTION_OBSERVER_H
#define PEER_CONNECTION_OBSERVER_H

#include "api/peer_connection_interface.h"
#include "rtc_sender/robot_webrtc_client.h"

namespace rtc_sender {
    namespace observers {
        class PeerConnectionObserver : public webrtc::PeerConnectionObserver {
        public:
            explicit PeerConnectionObserver(const std::shared_ptr<RobotWebRTCClient> &robot_webrtc_client,
                                            const std::shared_ptr<PeerConnectionManager> &peer_connection_manager)
                : robot_webrtc_client_(robot_webrtc_client), peer_connection_manager_(peer_connection_manager) {
            }

            ~PeerConnectionObserver() override = default;

            // Triggered when a remote peer opens a data channel.
            void OnDataChannel(webrtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;

            // A new ICE candidate has been gathered.
            void OnIceCandidate(const webrtc::IceCandidateInterface *candidate) override;

            // Called any time the standards-compliant IceConnectionState changes.
            void OnStandardizedIceConnectionChange(
                webrtc::PeerConnectionInterface::IceConnectionState new_state) override;

            // Called any time the IceGatheringState changes.
            void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) override;

            // Triggered when the SignalingState changed.
            void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) override;

            // Called any time the PeerConnectionState changes.
            void OnConnectionChange(webrtc::PeerConnectionInterface::PeerConnectionState new_state) override;

        private:
            std::shared_ptr<RobotWebRTCClient> robot_webrtc_client_;
            std::shared_ptr<PeerConnectionManager> peer_connection_manager_;
        };
    } // namespace observers
} // namespace rtc_sender

#endif  // PEER_CONNECTION_OBSERVER_H
