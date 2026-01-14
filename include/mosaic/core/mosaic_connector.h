//
// Created by yhkim on 25. 6. 3.
//

#ifndef MOSAIC_RTC_CORE_MOSAIC_CONNECTOR_H
#define MOSAIC_RTC_CORE_MOSAIC_CONNECTOR_H

#include <memory>
#include <string>

#include <mosaic/core/connector_state_manager.h>
#include <mosaic/core/peer_connection_manager.h>
#include <mosaic/handlers/data_channel/i_data_channel_handler.h>
#include <mosaic/handlers/media_track/i_media_track_handler.h>
#include <mosaic/rtc/webrtc_forward_decl.h>

namespace mosaic::core_observers {
class PeerConnectionObserver;
}  // namespace mosaic::core_observers

namespace mosaic::core {
// Forward declarations for avoid circular dependencies
class SignalingClient;
class PeerConnectionManager;

class MosaicConnector {
  public:
    explicit MosaicConnector(const std::string& robot_id,
                             const std::string& user_id,
                             const std::shared_ptr<ConnectorStateManager>& state_manager);

    ~MosaicConnector();

    // Disable copy constructor and assignment operator
    MosaicConnector(const MosaicConnector&) = delete;

    MosaicConnector& operator=(const MosaicConnector&) = delete;

    // Enable move constructor and assignment operator
    MosaicConnector(MosaicConnector&&) noexcept = default;

    MosaicConnector& operator=(MosaicConnector&&) noexcept = default;

    [[nodiscard]] std::string GetRobotId() const;

    [[nodiscard]] std::string GetUserId() const;

    void SetPeerConnectionManager(const std::shared_ptr<PeerConnectionManager>& peer_connection_manager) const;

    void InitializeWebRTC() const;

    void AddDataChannelHandler(const std::shared_ptr<handlers::IDataChannelHandler>& data_channel_handler) const;

    void AddMediaTrackHandler(const std::shared_ptr<handlers::IMediaTrackHandler>& media_track_handler) const;

    void HandleSdpOffer(webrtc::SessionDescriptionInterface* sdp_offer) const;

    void HandleIceCandidateFromSignaling(webrtc::IceCandidateInterface* candidate) const;

    void ClosePeerConnection() const;

    void ShuttingDown() const;

  private:
    class Impl;
    std::unique_ptr<Impl> pImpl;

    void CreateAllMediaTracks() const;

    void CloseAllMediaTracks() const;

    void CloseAllDataChannels() const;

    void OnDataChannel(const webrtc::scoped_refptr<webrtc::DataChannelInterface>& data_channel) const;

    [[nodiscard]] std::shared_ptr<ConnectorStateManager> GetStateManager() const;

    // Friends to access pImpl
    friend class MosaicConnectorFactory;
    friend class PeerConnectionManager;
    friend class SignalingClient;
    friend class core_observers::PeerConnectionObserver;

    [[nodiscard]] Impl* GetImpl() const {
        return pImpl.get();
    }
};
}  // namespace mosaic::core

#endif  // MOSAIC_RTC_CORE_MOSAIC_CONNECTOR_H
