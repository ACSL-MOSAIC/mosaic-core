//
// Created by yhkim on 25. 6. 2.
//

#ifndef BA_GCS_RTC_SENDER_SIGNALING_CLIENT_H
#define BA_GCS_RTC_SENDER_SIGNALING_CLIENT_H

#include <memory>
#include <string>

#include "gcs_connector.h"

// Forward declarations for PIMPL
namespace webrtc {
  class SessionDescriptionInterface;
  class IceCandidateInterface;
} // namespace webrtc

namespace rtc_sender {
  class SignalingClient {
  public:
    explicit SignalingClient(const std::string &ws_uri);

    ~SignalingClient();

    void SetRobotWebRTCClient(const std::shared_ptr<GCSConnector> &robot_webrtc_client) const;

    void Start();

    void Stop();

    void SendSdpAnswer(const webrtc::SessionDescriptionInterface *sdp) const;

    void SendIceCandidate(const webrtc::IceCandidateInterface *candidate) const;

    void SendState(const std::string &state) const;

  private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
  };
} // namespace rtc_sender
#endif  // BA_GCS_RTC_SENDER_SIGNALING_CLIENT_H
