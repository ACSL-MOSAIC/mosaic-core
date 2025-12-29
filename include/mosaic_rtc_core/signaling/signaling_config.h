//
// Created by yhkim on 25. 7. 2.
//

#ifndef BA_GCS_RTC_SENDER_SIGNALING_CONFIG_H
#define BA_GCS_RTC_SENDER_SIGNALING_CONFIG_H

#include <string>
#include <utility>

namespace rtc_sender::signaling {
    struct SignalingConfig {
        std::string ws_uri; // WebSocket URI for signaling server
        std::string robot_id; // Unique identifier for the robot

        SignalingConfig(std::string ws_uri, std::string robot_id) : ws_uri(std::move(ws_uri)),
                                                                    robot_id(std::move(robot_id)) {
        }

        [[nodiscard]] std::string GetSignalingUrl() const {
            return ws_uri + "/ws/robot?robot_id=" + robot_id;
        }
    };
} // namespace rtc_sender::signaling

#endif  // BA_GCS_RTC_SENDER_SIGNALING_CONFIG_H
