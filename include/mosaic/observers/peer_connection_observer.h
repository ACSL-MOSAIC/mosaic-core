//
// Created by yhkim on 25. 6. 5.
//

#ifndef MOSAIC_RTC_CORE_PEER_CONNECTION_OBSERVER_H
#define MOSAIC_RTC_CORE_PEER_CONNECTION_OBSERVER_H

#include <api/peer_connection_interface.h>
#include <mosaic/core/mosaic_connector.h>

namespace mosaic::core_observers {
class PeerConnectionObserver : public webrtc::PeerConnectionObserver {
  public:
    explicit PeerConnectionObserver(const std::shared_ptr<core::MosaicConnector>& gcs_connector,
                                    const std::shared_ptr<core::PeerConnectionManager>& pc_manager)
        : gcs_connector_(gcs_connector), pc_manager_(pc_manager) {}

    ~PeerConnectionObserver() override = default;

    // Triggered when a remote peer opens a data channel.
    void OnDataChannel(webrtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;

    // A new ICE candidate has been gathered.
    void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override;

    // Called any time the standards-compliant IceConnectionState changes.
    void OnStandardizedIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state) override;

    // Called any time the IceGatheringState changes.
    void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) override;

    // Triggered when the SignalingState changed.
    void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) override;

    // Called any time the PeerConnectionState changes.
    void OnConnectionChange(webrtc::PeerConnectionInterface::PeerConnectionState new_state) override;

  private:
    std::shared_ptr<core::MosaicConnector> gcs_connector_;
    std::shared_ptr<core::PeerConnectionManager> pc_manager_;
};
}  // namespace mosaic::core_observers

#endif  // MOSAIC_RTC_CORE_PEER_CONNECTION_OBSERVER_H
