//
// Created by yhkim on 12/28/25.
//

#include <mosaic/auto_configurer/config_reader/yaml_config_reader.h>
#include <yaml-cpp/yaml.h>

using namespace mosaic::core;
using namespace mosaic::auto_configurer;

class YamlConfigReader::Impl {
  public:
    Impl() = default;

    YAML::Node LoadFile(const std::string& file_path) {
        return YAML::LoadFile(file_path);
    }

    ConnectorConfigs ParseConnectorConfigs(const YAML::Node& node) {
        ConnectorConfigs configs;

        if (node["server"]) {
            configs.server = ParseServerConfig(node["server"]);
        } else {
            throw std::invalid_argument("Server config is not found.");
        }

        if (node["connectors"]) {
            for (const auto& connector_node : node["connectors"]) {
                configs.connectors.push_back(ParseConnectorConfig(connector_node));
            }
        } else {
            throw std::invalid_argument("Connectors config is not found.");
        }

        return configs;
    }

    ConnectorConfig ParseConnectorConfig(const YAML::Node& node) {
        ConnectorConfig config;

        if (node["type"]) {
            config.type = node["type"].as<std::string>();
        } else {
            throw std::invalid_argument("Connector type is not found.");
        }

        if (node["label"]) {
            config.label = node["label"].as<std::string>();
        } else {
            throw std::invalid_argument("Connector label is not found.");
        }

        if (node["params"]) {
            for (const auto& param : node["params"]) {
                config.params[param.first.as<std::string>()] = param.second.as<std::string>();
            }
        }

        return config;
    }

    ServerConfig ParseServerConfig(const YAML::Node& node) {
        ServerConfig config;

        if (node["ws_url"]) {
            config.ws_url = node["ws_url"].as<std::string>();
        } else {
            throw std::invalid_argument("Server websocket url is not found.");
        }

        if (node["auth"]) {
            config.auth_config = ParseAuthConfig(node["auth"]);
        } else {
            throw std::invalid_argument("Server auth config is not found.");
        }

        if (node["webrtc"]) {
            config.webrtc_config = ParseWebRtcConfig(node["webrtc"]);
        } else {
            throw std::invalid_argument("Server webrtc config is not found.");
        }

        return config;
    }

    AuthConfig ParseAuthConfig(const YAML::Node& node) {
        AuthConfig config;

        if (node["type"]) {
            config.type = node["type"].as<std::string>();
        } else {
            throw std::invalid_argument("Auth type is not found.");
        }

        if (node["robot_id"]) {
            config.robot_id = node["robot_id"].as<std::string>();
        } else {
            throw std::invalid_argument("Robot id is not found.");
        }

        if (node["user_id"]) {
            config.user_id = node["user_id"].as<std::string>();
        } else {
            throw std::invalid_argument("User id is not found.");
        }

        if (node["params"]) {
            for (const auto& param : node["params"]) {
                config.params[param.first.as<std::string>()] = param.second.as<std::string>();
            }
        }

        return config;
    }

    WebRtcConfig ParseWebRtcConfig(const YAML::Node& node) {
        WebRtcConfig config;

        if (node["ice_servers"]) {
            config.ice_servers = ParseIceServers(node["ice_servers"]);
        } else {
            throw std::invalid_argument("WebRTC ice servers are not found.");
        }

        return config;
    }

    std::vector<IceServerConfig> ParseIceServers(const YAML::Node& node) {
        std::vector<IceServerConfig> ice_servers;

        for (const auto& ice_server_node : node) {
            IceServerConfig ice_server;

            if (ice_server_node["urls"]) {
                for (const auto& url : ice_server_node["urls"]) {
                    ice_server.urls.push_back(url.as<std::string>());
                }
            } else {
                throw std::invalid_argument("Ice server urls are not found.");
            }

            if (ice_server_node["username"]) {
                ice_server.username = ice_server_node["username"].as<std::string>();
            } else {
                throw std::invalid_argument("Ice server username is not found.");
            }

            if (ice_server_node["credential"]) {
                ice_server.credential = ice_server_node["credential"].as<std::string>();
            } else {
                throw std::invalid_argument("Ice server credential is not found.");
            }

            ice_servers.push_back(ice_server);
        }

        return ice_servers;
    }
};

YamlConfigReader::YamlConfigReader() : pImpl(std::make_unique<Impl>()) {}

YamlConfigReader::~YamlConfigReader() = default;

bool YamlConfigReader::IsSupportedExtension(const std::string& extension) {
    return extension == "yaml" || extension == "yml";
}

ConnectorConfigs YamlConfigReader::LoadConfigs(const std::string& file_path) {
    const YAML::Node root = pImpl->LoadFile(file_path);
    return pImpl->ParseConnectorConfigs(root);
}
