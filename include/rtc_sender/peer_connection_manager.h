//
// Created by yhkim on 25. 7. 2.
//

#ifndef BA_GCS_RTC_SENDER_PEER_CONNECTION_MANAGER_H
#define BA_GCS_RTC_SENDER_PEER_CONNECTION_MANAGER_H

#include <vector>

#include "gcs_connector.h"
#include "webrtc_forward_decl.h"
#include "signaling/i_signaling_client.h"

using namespace rtc_sender::signaling;

namespace rtc_sender {
    // Forward declarations to avoid circular dependencies
    class ConnectorStateManager;
    struct IceConfig;
    class GCSConnector;

    class PeerConnectionManager : public std::enable_shared_from_this<PeerConnectionManager> {
    public:
        PeerConnectionManager(const std::shared_ptr<GCSConnector> &client,
                              const std::shared_ptr<ConnectorStateManager> &state_manager,
                              const std::shared_ptr<ISignalingClient> &signaling_client,
                              const std::shared_ptr<IceConfig> &ice_config);

        void InitializeWebRTC() const;

        void CreatePeerConnection();

        void HandleSdpOffer(webrtc::SessionDescriptionInterface *sdp_offer);

        void AfterSetSessionDescription();

        void AfterCreateSdpAnswer(webrtc::SessionDescriptionInterface *desc);

        void AfterSetLocalDescription(const webrtc::SessionDescriptionInterface *desc) const;

        void HandleIceCandidateFromSignaling(const webrtc::IceCandidateInterface *candidate) const;

        void OnConnectionConnected() const;

        void OnConnectionDisconnected() const;

        void SendIceCandidate(const webrtc::IceCandidateInterface *candidate) const;

        void StopSignalingServer() const;

        void ClosePeerConnection() const;

        void DestroyPeerConnectionManager() const;

    private:
        class Impl;
        std::shared_ptr<Impl> pImpl;

        friend class GCSConnector;

        webrtc::scoped_refptr<webrtc::VideoTrackInterface> CreateVideoTrack(
            const webrtc::scoped_refptr<webrtc::VideoTrackSourceInterface> &source,
            const std::string &label) const;

        webrtc::scoped_refptr<webrtc::RtpSenderInterface> AddTrack(
            const webrtc::scoped_refptr<webrtc::MediaStreamTrackInterface> &track,
            const std::vector<std::string> &stream_ids) const;

        webrtc::scoped_refptr<webrtc::PeerConnectionInterface> GetPeerConnection() const;

        Impl *GetImpl() const {
            return pImpl.get();
        }
    };
} // namespace rtc_sender

#endif  // BA_GCS_RTC_SENDER_PEER_CONNECTION_MANAGER_H
