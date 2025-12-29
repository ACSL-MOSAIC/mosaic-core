//
// Created by yhkim on 25. 6. 2.
//

#ifndef BA_GCS_RTC_SENDER_SIGNALING_CLIENT_H
#define BA_GCS_RTC_SENDER_SIGNALING_CLIENT_H

#include <memory>
#include <string>

#include <mosaic_rtc_core/gcs_connector.h>
#include <mosaic_rtc_core/signaling/i_signaling_client.h>

namespace rtc_sender::signaling {
  class SignalingClient : public ISignalingClient {
  public:
    explicit SignalingClient(const std::string &ws_uri);

    ~SignalingClient() override;

    void Start() override;

    void Stop() override;

    void SendSdpAnswer(const webrtc::SessionDescriptionInterface *sdp) const override;

    void SendIceCandidate(const webrtc::IceCandidateInterface *candidate) const override;

    void SendState(const std::string &state) const override;

    void SetGCSConnector(const std::shared_ptr<GCSConnector> &gcs_connector) const override;

    void SetAuthenticator(const std::shared_ptr<security::IGCSAuthenticator> &authenticator) const override;

    [[nodiscard]] bool IsAuthenticated() const override;

  private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
  };
} // namespace rtc_sender::signaling

#endif  // BA_GCS_RTC_SENDER_SIGNALING_CLIENT_H
