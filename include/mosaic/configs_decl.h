//
// Created by yhkim on 12/28/25.
//

#ifndef MOSAIC_CORE_CONFIGS_DECL_H
#define MOSAIC_CORE_CONFIGS_DECL_H

#include <string>
#include <unordered_map>
#include <vector>

namespace mosaic::core {
struct IceServerConfig {
    std::vector<std::string> urls;
    std::string username;
    std::string credential;
};

struct WebRtcConfig {
    std::vector<IceServerConfig> ice_servers;
};

struct AuthConfig {
    std::string type;
    std::string robot_id;
    std::unordered_map<std::string, std::string> params;
};

struct ConnectorConfig {
    std::string type;
    std::string label;
    std::unordered_map<std::string, std::string> params;
};

struct ServerConfig {
    std::string ws_url;
    AuthConfig auth_config;
    WebRtcConfig webrtc_config;

    [[nodiscard]] std::string GetSignalingUrl() const {
        return ws_url + "/ws/robot";
    }
};

struct MosaicConfig {
    ServerConfig server;
    std::vector<ConnectorConfig> connectors;
};
}  // namespace mosaic::core

#endif  // MOSAIC_CORE_CONFIGS_DECL_H
