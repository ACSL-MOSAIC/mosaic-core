//
// Created by yhkim on 25. 6. 3.
//

#ifndef ROBOT_WEBRTC_CLIENT_H
#define ROBOT_WEBRTC_CLIENT_H
#include <memory>
#include <string>

#include "client_state_manager.h"
#include "handlers/data_channel/a_data_channel_handler.h"
#include "handlers/media_track/media_track_handler.h"
#include "peer_connection_manager.h"
#include "webrtc_forward_decl.h"

namespace rtc_sender {
    // Forward declarations for avoid circular dependencies
    class SignalingServer;
    class PeerConnectionManager;

    namespace observers {
        class PeerConnectionObserver;
    }

    class RobotWebRTCClient {
    public:
        explicit RobotWebRTCClient(const std::string &robot_id,
                                   const std::string &user_id,
                                   const std::shared_ptr<ClientStateManager> &state_manager);

        ~RobotWebRTCClient();

        // Disable copy constructor and assignment operator
        RobotWebRTCClient(const RobotWebRTCClient &) = delete;

        RobotWebRTCClient &operator=(const RobotWebRTCClient &) = delete;

        // Enable move constructor and assignment operator
        RobotWebRTCClient(RobotWebRTCClient &&) noexcept = default;

        RobotWebRTCClient &operator=(RobotWebRTCClient &&) noexcept = default;

        std::string GetRobotId() const;

        std::string GetUserId() const;

        void SetPeerConnectionManager(const std::shared_ptr<PeerConnectionManager> &peer_connection_manager) const;

        void InitializeWebRTC() const;

        void AddDataChannelHandler(const std::shared_ptr<handlers::ADataChannelHandler> &data_channel_handler) const;

        void AddMediaTrackHandler(const std::shared_ptr<handlers::IMediaTrackHandler> &media_track_handler) const;

        void HandleSdpOffer(webrtc::SessionDescriptionInterface *sdp_offer) const;

        void HandleIceCandidateFromSignaling(webrtc::IceCandidateInterface *candidate) const;

        void ClosePeerConnection() const;

        void ShuttingDown() const;

    private:
        class Impl;
        std::unique_ptr<Impl> pImpl;

        void CreateAllMediaTracks() const;

        void CreateAllDataChannels() const;

        void CloseAllMediaTracks() const;

        void CloseAllDataChannels() const;

        void OnDataChannel(webrtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) const;

        std::shared_ptr<ClientStateManager> GetStateManager() const;

        // Friends to access pImpl
        friend class RobotWebRTCClientFactory;
        friend class PeerConnectionManager;
        friend class SignalingServer;
        friend class observers::PeerConnectionObserver;

        Impl *getImpl() const {
            return pImpl.get();
        }
    };
} // namespace rtc_sender

#endif  // ROBOT_WEBRTC_CLIENT_H
