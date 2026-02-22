//
// Created by yhkim on 12/25/25.
//

#ifndef MOSAIC_CORE_I_SIGNALING_CLIENT_HPP
#define MOSAIC_CORE_I_SIGNALING_CLIENT_HPP

#include "../rtc/webrtc_forward_decl.hpp"
#include "../security/mosaic_authenticator.hpp"

namespace mosaic::core {
// TODO: forward decleration 필요한지 확
class MosaicConnector;
}  // namespace mosaic::core

namespace mosaic::core_signaling {
class ISignalingClient {
  public:
    virtual ~ISignalingClient() = default;

    virtual void Start() = 0;

    virtual void Stop() = 0;

    virtual void SendSdpAnswer(const webrtc::SessionDescriptionInterface* sdp) const = 0;

    virtual void SendIceCandidate(const webrtc::IceCandidateInterface* candidate) const = 0;

    virtual void SendState(int state) const = 0;

    virtual void SetMosaicConnector(const std::shared_ptr<core::MosaicConnector>& mosaic_connector) = 0;

    virtual void SetAuthenticator(const std::shared_ptr<security::IMosaicAuthenticator>& authenticator) = 0;

    virtual bool IsAuthenticated() const = 0;

    virtual void SendWsMessage(const Json::Value& message) const = 0;

    virtual void HandlePingPong(Json::Value const& message) const = 0;

    virtual void HandlePrepareConnection(Json::Value const& message) = 0;

    virtual void HandleSendSdpOffer(Json::Value const& message) const = 0;

    virtual void HandleExchangeIceCandidate(Json::Value const& message) const = 0;

    virtual void HandleCloseConnection() const = 0;

    virtual void HandleAuthorizationMessage(const std::string& type, Json::Value const& message) const = 0;
};
}  // namespace mosaic::core_signaling

#endif  // MOSAIC_CORE_I_SIGNALING_CLIENT_HPP
