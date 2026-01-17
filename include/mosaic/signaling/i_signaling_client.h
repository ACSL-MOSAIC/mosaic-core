//
// Created by yhkim on 12/25/25.
//

#ifndef MOSAIC_RTC_CORE_I_SIGNALING_CLIENT_H
#define MOSAIC_RTC_CORE_I_SIGNALING_CLIENT_H

#include <string>

#include <mosaic/rtc/webrtc_forward_decl.h>
#include <mosaic/security/i_mosaic_authenticator.h>

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

    virtual void SendState(const std::string& state) const = 0;

    virtual void SetMosaicConnector(const std::shared_ptr<core::MosaicConnector>& mosaic_connector) const = 0;

    virtual void SetAuthenticator(const std::shared_ptr<security::IMosaicAuthenticator>& authenticator) const = 0;

    [[nodiscard]] virtual bool IsAuthenticated() const = 0;
};
}  // namespace mosaic::core_signaling

#endif  // MOSAIC_RTC_CORE_I_SIGNALING_CLIENT_H
