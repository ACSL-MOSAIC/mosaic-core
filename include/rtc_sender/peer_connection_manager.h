//
// Created by yhkim on 25. 7. 2.
//

#ifndef PEER_CONNECTION_MANAGER_H
#define PEER_CONNECTION_MANAGER_H

#include <vector>

#include "robot_webrtc_client.h"
#include "webrtc_forward_decl.h"

namespace rtc_sender {
    // Forward declarations to avoid circular dependencies
    class ClientStateManager;
    struct IceConfig;
    class SignalingServer;
    class RobotWebRTCClient;

    class PeerConnectionManager : public std::enable_shared_from_this<PeerConnectionManager> {
    public:
        PeerConnectionManager(const std::shared_ptr<RobotWebRTCClient> &client,
                              const std::shared_ptr<ClientStateManager> &state_manager,
                              const std::shared_ptr<SignalingServer> &signaling_server,
                              const std::shared_ptr<IceConfig> &ice_config);

        void InitializeWebRTC();

        void CreatePeerConnection();

        void HandleSdpOffer(webrtc::SessionDescriptionInterface *sdp_offer);

        void AfterSetSessionDescription();

        void AfterCreateSdpAnswer(webrtc::SessionDescriptionInterface *desc);

        void AfterSetLocalDescription(const webrtc::SessionDescriptionInterface *desc) const;

        void HandleIceCandidateFromSignaling(const webrtc::IceCandidateInterface *candidate) const;

        void OnConnectionConnected() const;

        void OnConnectionDisconnected() const;

        void SendIceCandidate(const webrtc::IceCandidateInterface *candidate) const;

        void StopSignalingServer();

        void ClosePeerConnection();

        void DestroyPeerConnectionManager();

    private:
        class Impl;
        std::shared_ptr<Impl> pImpl;

        friend class RobotWebRTCClient;

        webrtc::scoped_refptr<webrtc::VideoTrackInterface> CreateVideoTrack(
            webrtc::scoped_refptr<webrtc::VideoTrackSourceInterface> source,
            std::string label);

        webrtc::scoped_refptr<webrtc::RtpSenderInterface> AddTrack(
            webrtc::scoped_refptr<webrtc::MediaStreamTrackInterface> track,
            const std::vector<std::string> &stream_ids);

        webrtc::scoped_refptr<webrtc::PeerConnectionInterface> GetPeerConnection();

        Impl *getImpl() const {
            return pImpl.get();
        }
    };
} // namespace rtc_sender

#endif  // PEER_CONNECTION_MANAGER_H
