//
// Created by yhkim on 12/25/25.
//

#ifndef BA_GCS_RTC_SENDER_I_SIGNALING_CLIENT_H
#define BA_GCS_RTC_SENDER_I_SIGNALING_CLIENT_H

#include <string>

#include "rtc_sender/webrtc_forward_decl.h"
#include "rtc_sender/security/i_gcs_authenticator.h"

namespace rtc_sender {
    class GCSConnector;
}

namespace rtc_sender::signaling {
    class ISignalingClient {
    public:
        virtual ~ISignalingClient() = default;

        virtual void Start() = 0;

        virtual void Stop() = 0;

        virtual void SendSdpAnswer(const webrtc::SessionDescriptionInterface *sdp) const = 0;

        virtual void SendIceCandidate(const webrtc::IceCandidateInterface *candidate) const = 0;

        virtual void SendState(const std::string &state) const = 0;

        virtual void SetGCSConnector(const std::shared_ptr<GCSConnector> &gcs_connector) const = 0;

        virtual void SetAuthenticator(const std::shared_ptr<security::IGCSAuthenticator> &authenticator) const = 0;

        virtual bool IsAuthenticated() const = 0;
    };
} // namespace rtc_sender::signaling

#endif //BA_GCS_RTC_SENDER_I_SIGNALING_CLIENT_H
