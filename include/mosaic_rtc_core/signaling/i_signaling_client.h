//
// Created by yhkim on 12/25/25.
//

#ifndef MOSAIC_RTC_CORE_I_SIGNALING_CLIENT_H
#define MOSAIC_RTC_CORE_I_SIGNALING_CLIENT_H

#include <string>

#include <mosaic_rtc_core/security/i_gcs_authenticator.h>
#include <mosaic_rtc_core/webrtc_forward_decl.h>

namespace mosaic::core {
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

    virtual void SetGCSConnector(const std::shared_ptr<core::MosaicConnector>& gcs_connector) const = 0;

    virtual void SetAuthenticator(const std::shared_ptr<security::IGCSAuthenticator>& authenticator) const = 0;

    [[nodiscard]] virtual bool IsAuthenticated() const = 0;
};
}  // namespace mosaic::core_signaling

#endif  // MOSAIC_RTC_CORE_I_SIGNALING_CLIENT_H
