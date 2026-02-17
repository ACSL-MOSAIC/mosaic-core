//
// Created by yhkim on 25. 6. 3.
//

#ifndef MOSAIC_CORE_MOSAIC_CONNECTOR_HPP
#define MOSAIC_CORE_MOSAIC_CONNECTOR_HPP

#include <map>
#include <memory>
#include <string>

#include "../handlers/data_channel/data_channel_handler.hpp"
#include "../handlers/media_track/media_track_handler.hpp"
#include "connector_state_manager.hpp"
#include "peer_connection_manager.hpp"

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

class MosaicConnectorFactory {
  public:
    explicit MosaicConnectorFactory(const std::shared_ptr<ServerConfig>& server_config)
        : server_config_(server_config) {}

    std::shared_ptr<MosaicConnector> Create() const;

  private:
    std::shared_ptr<ServerConfig> server_config_;
};

}  // namespace mosaic::core

#endif  // MOSAIC_CORE_MOSAIC_CONNECTOR_HPP
