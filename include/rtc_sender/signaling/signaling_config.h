//
// Created by yhkim on 25. 7. 2.
//

#ifndef BA_GCS_RTC_SENDER_SIGNALING_CONFIG_H
#define BA_GCS_RTC_SENDER_SIGNALING_CONFIG_H

#include <string>

namespace rtc_sender::signaling {
    struct SignalingConfig {
        std::string ws_uri; // WebSocket URI for signaling server
        std::string robot_id; // Unique identifier for the robot

        SignalingConfig(const std::string &ws_uri, const std::string &robot_id) : ws_uri(ws_uri), robot_id(robot_id) {
        }

        std::string GetSignalingUrl() const {
            return ws_uri + "/ws/robot?robot_id=" + robot_id;
        }
    };
} // namespace rtc_sender::signaling

#endif  // BA_GCS_RTC_SENDER_SIGNALING_CONFIG_H
