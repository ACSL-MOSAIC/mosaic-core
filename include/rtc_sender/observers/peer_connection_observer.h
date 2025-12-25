//
// Created by yhkim on 25. 6. 5.
//

#ifndef BA_GCS_RTC_SENDER_PEER_CONNECTION_OBSERVER_H
#define BA_GCS_RTC_SENDER_PEER_CONNECTION_OBSERVER_H

#include "api/peer_connection_interface.h"
#include "rtc_sender/gcs_connector.h"

namespace rtc_sender::observers {
    class PeerConnectionObserver : public webrtc::PeerConnectionObserver {
    public:
        explicit PeerConnectionObserver(const std::shared_ptr<GCSConnector> &gcs_connector,
                                        const std::shared_ptr<PeerConnectionManager> &pc_manager)
            : gcs_connector_(gcs_connector), pc_manager_(pc_manager) {
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
        std::shared_ptr<GCSConnector> gcs_connector_;
        std::shared_ptr<PeerConnectionManager> pc_manager_;
    };
} // namespace rtc_sender::observers

#endif  // BA_GCS_RTC_SENDER_PEER_CONNECTION_OBSERVER_H
