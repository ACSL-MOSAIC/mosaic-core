//
// Created by yhkim on 25. 6. 3.
//

#ifndef BA_GCS_RTC_SENDER_GCS_CONNECTOR_H
#define BA_GCS_RTC_SENDER_GCS_CONNECTOR_H

#include <memory>
#include <string>

#include <rtc_sender/connector_state_manager.h>
#include <rtc_sender/peer_connection_manager.h>
#include <rtc_sender/webrtc_forward_decl.h>
#include <rtc_sender/handlers/media_track/i_media_track_handler.h>
#include <rtc_sender/handlers/data_channel/i_data_channel_handler.h>

namespace rtc_sender {
    // Forward declarations for avoid circular dependencies
    class SignalingClient;
    class PeerConnectionManager;

    namespace observers {
        class PeerConnectionObserver;
    }

    class GCSConnector {
    public:
        explicit GCSConnector(const std::string &robot_id,
                              const std::string &user_id,
                              const std::shared_ptr<ConnectorStateManager> &state_manager);

        ~GCSConnector();

        // Disable copy constructor and assignment operator
        GCSConnector(const GCSConnector &) = delete;

        GCSConnector &operator=(const GCSConnector &) = delete;

        // Enable move constructor and assignment operator
        GCSConnector(GCSConnector &&) noexcept = default;

        GCSConnector &operator=(GCSConnector &&) noexcept = default;

        [[nodiscard]] std::string GetRobotId() const;

        [[nodiscard]] std::string GetUserId() const;

        void SetPeerConnectionManager(const std::shared_ptr<PeerConnectionManager> &peer_connection_manager) const;

        void InitializeWebRTC() const;

        void AddDataChannelHandler(const std::shared_ptr<handlers::IDataChannelHandler> &data_channel_handler) const;

        void AddMediaTrackHandler(const std::shared_ptr<handlers::IMediaTrackHandler> &media_track_handler) const;

        void HandleSdpOffer(webrtc::SessionDescriptionInterface *sdp_offer) const;

        void HandleIceCandidateFromSignaling(webrtc::IceCandidateInterface *candidate) const;

        void ClosePeerConnection() const;

        void ShuttingDown() const;

    private:
        class Impl;
        std::unique_ptr<Impl> pImpl;

        void CreateAllMediaTracks() const;

        void CloseAllMediaTracks() const;

        void CloseAllDataChannels() const;

        void OnDataChannel(const webrtc::scoped_refptr<webrtc::DataChannelInterface> &data_channel) const;

        [[nodiscard]] std::shared_ptr<ConnectorStateManager> GetStateManager() const;

        // Friends to access pImpl
        friend class GCSConnectorFactory;
        friend class PeerConnectionManager;
        friend class SignalingClient;
        friend class observers::PeerConnectionObserver;

        [[nodiscard]] Impl *GetImpl() const {
            return pImpl.get();
        }
    };
} // namespace rtc_sender

#endif  // BA_GCS_RTC_SENDER_GCS_CONNECTOR_H
