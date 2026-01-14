//
// Created by yhkim on 25. 7. 2.
//

#ifndef MOSAIC_RTC_CORE_PEER_CONNECTION_MANAGER_H
#define MOSAIC_RTC_CORE_PEER_CONNECTION_MANAGER_H

#include <vector>

#include <mosaic_rtc_core/core/mosaic_connector.h>
#include <mosaic_rtc_core/signaling/i_signaling_client.h>

using namespace mosaic::core_signaling;

namespace mosaic::core {
// Forward declarations to avoid circular dependencies
class ConnectorStateManager;
struct IceConfig;
class MosaicConnector;

class PeerConnectionManager : public std::enable_shared_from_this<PeerConnectionManager> {
  public:
    PeerConnectionManager(const std::shared_ptr<MosaicConnector>& client,
                          const std::shared_ptr<ConnectorStateManager>& state_manager,
                          const std::shared_ptr<ISignalingClient>& signaling_client,
                          const std::shared_ptr<IceConfig>& ice_config);

    void InitializeWebRTC() const;

    void CreatePeerConnection();

    void HandleSdpOffer(webrtc::SessionDescriptionInterface* sdp_offer);

    void AfterSetSessionDescription();

    void AfterCreateSdpAnswer(webrtc::SessionDescriptionInterface* desc);

    void AfterSetLocalDescription(const webrtc::SessionDescriptionInterface* desc) const;

    void HandleIceCandidateFromSignaling(const webrtc::IceCandidateInterface* candidate) const;

    void OnConnectionConnected() const;

    void OnConnectionDisconnected() const;

    void SendIceCandidate(const webrtc::IceCandidateInterface* candidate) const;

    void StopSignalingServer() const;

    void ClosePeerConnection() const;

    void DestroyPeerConnectionManager() const;

  private:
    class Impl;
    std::shared_ptr<Impl> pImpl;

    friend class MosaicConnector;

    webrtc::scoped_refptr<webrtc::VideoTrackInterface> CreateVideoTrack(
        const webrtc::scoped_refptr<webrtc::VideoTrackSourceInterface>& source,
        const std::string& label) const;

    webrtc::scoped_refptr<webrtc::RtpSenderInterface> AddTrack(
        const webrtc::scoped_refptr<webrtc::MediaStreamTrackInterface>& track,
        const std::vector<std::string>& stream_ids) const;

    webrtc::scoped_refptr<webrtc::PeerConnectionInterface> GetPeerConnection() const;

    Impl* GetImpl() const {
        return pImpl.get();
    }
};
}  // namespace mosaic::core

#endif  // MOSAIC_RTC_CORE_PEER_CONNECTION_MANAGER_H
