//
// Created by yhkim on 25. 6. 3.
//

#ifndef MOSAIC_RTC_CORE_MOSAIC_CONNECTOR_H
#define MOSAIC_RTC_CORE_MOSAIC_CONNECTOR_H

#include <map>
#include <memory>
#include <string>

#include <mosaic/core/connector_state_manager.h>
#include <mosaic/core/peer_connection_manager.h>
#include <mosaic/handlers/data_channel/i_data_channel_handler.h>
#include <mosaic/handlers/media_track/i_media_track_handler.h>
#include <mosaic/rtc/webrtc_forward_decl.h>

namespace mosaic::core_signaling {
class SignalingClient;
}  // namespace mosaic::core_signaling

namespace mosaic::core {
// Forward declarations for avoid circular dependencies
class PeerConnectionManager;
class PeerConnectionObserver;

class MosaicConnector {
  public:
    explicit MosaicConnector(const std::shared_ptr<ConnectorStateManager>& state_manager);

    ~MosaicConnector();

    // Disable copy constructor and assignment operator
    MosaicConnector(const MosaicConnector&) = delete;

    MosaicConnector& operator=(const MosaicConnector&) = delete;

    // Enable move constructor and assignment operator
    MosaicConnector(MosaicConnector&&) noexcept = default;

    MosaicConnector& operator=(MosaicConnector&&) noexcept = default;

    void InitializeWebRTC() const;

    void AddDataChannelHandler(const std::shared_ptr<handlers::IDataChannelHandler>& data_channel_handler) const;

    void AddMediaTrackHandler(const std::shared_ptr<handlers::IMediaTrackHandler>& media_track_handler) const;

    void ShuttingDown() const;

  private:
    std::shared_ptr<PeerConnectionManager> peer_connection_manager_ = nullptr;
    std::shared_ptr<ConnectorStateManager> state_manager_;

    // 데이터 채널 핸들러를 label 과 함께 저장합니다.
    std::map<std::string, std::shared_ptr<handlers::IDataChannelHandler>> data_channel_handlers_dict_ = {};
    // 미디어 트랙 핸들러를 track name 과 함께 저장합니다.
    std::map<std::string, std::shared_ptr<handlers::IMediaTrackHandler>> media_track_handlers_dict_ = {};

    void CreateAllMediaTracks() const;

    void CloseAllMediaTracks() const;

    void CloseAllDataChannels() const;

    void OnDataChannel(const webrtc::scoped_refptr<webrtc::DataChannelInterface>& data_channel) const;

    std::shared_ptr<ConnectorStateManager> GetStateManager() const;

    void SetPeerConnectionManager(const std::shared_ptr<PeerConnectionManager>& peer_connection_manager) const;

    void HandleSdpOffer(webrtc::SessionDescriptionInterface* sdp_offer) const;

    void HandleIceCandidateFromSignaling(webrtc::IceCandidateInterface* candidate) const;

    void ClosePeerConnection() const;

    // Friends to access private members
    friend class MosaicConnectorFactory;
    friend class PeerConnectionManager;
    friend class core_signaling::SignalingClient;
    friend class PeerConnectionObserver;
};
}  // namespace mosaic::core

#endif  // MOSAIC_RTC_CORE_MOSAIC_CONNECTOR_H
