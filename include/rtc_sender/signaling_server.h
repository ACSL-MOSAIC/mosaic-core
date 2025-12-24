//
// Created by yhkim on 25. 6. 2.
//

#ifndef SIGNALING_SERVER_H
#define SIGNALING_SERVER_H

#include <memory>
#include <string>

#include "robot_webrtc_client.h"

// Forward declarations for PIMPL
namespace webrtc {
  class SessionDescriptionInterface;
  class IceCandidateInterface;
} // namespace webrtc

namespace rtc_sender {
  class SignalingServer {
  public:
    explicit SignalingServer(const std::string &ws_uri);

    ~SignalingServer();

    void SetRobotWebRTCClient(const std::shared_ptr<RobotWebRTCClient> &robot_webrtc_client) const;

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
#endif  // SIGNALING_SERVER_H
