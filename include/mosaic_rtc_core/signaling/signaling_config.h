//
// Created by yhkim on 25. 7. 2.
//

#ifndef MOSAIC_RTC_CORE_SIGNALING_CONFIG_H
#define MOSAIC_RTC_CORE_SIGNALING_CONFIG_H

#include <string>
#include <utility>

namespace mosaic::core_signaling {
struct SignalingConfig {
    std::string ws_uri;    // WebSocket URI for signaling server
    std::string robot_id;  // Unique identifier for the robot

    SignalingConfig(std::string ws_uri, std::string robot_id)
        : ws_uri(std::move(ws_uri)), robot_id(std::move(robot_id)) {}

    [[nodiscard]] std::string GetSignalingUrl() const {
        return ws_uri + "/ws/robot?robot_id=" + robot_id;
    }
};
}  // namespace mosaic::core_signaling

#endif  // MOSAIC_RTC_CORE_SIGNALING_CONFIG_H
