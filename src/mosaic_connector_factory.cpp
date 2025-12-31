//
// Created by yhkim on 25. 7. 7.
//

#include <mosaic_rtc_core/ice_config.h>
#include <mosaic_rtc_core/mosaic_connector_factory.h>
#include <mosaic_rtc_core/signaling/signaling_client.h>

using namespace mosaic::core;

MosaicConnectorFactory::MosaicConnectorFactory(const std::string& robot_id,
                                               const std::string& user_id,
                                               const std::shared_ptr<core_signaling::SignalingConfig>& signaling_config,
                                               const std::shared_ptr<IceConfig>& ice_config) {
    robot_id_ = robot_id;
    user_id_ = user_id;
    signaling_config_ = signaling_config;
    ice_config_ = ice_config;
}

MosaicConnectorFactory::MosaicConnectorFactory(const std::string& robot_id,
                                               const std::string& user_id,
                                               const std::string& ws_uri,
                                               const std::string& turn_url,
                                               const std::string& turn_username,
                                               const std::string& turn_credential) {
    robot_id_ = robot_id;
    user_id_ = user_id;
    signaling_config_ = std::make_shared<core_signaling::SignalingConfig>(ws_uri, robot_id);
    ice_config_ = std::make_shared<IceConfig>(turn_url, turn_username, turn_credential);
}

std::shared_ptr<MosaicConnector> MosaicConnectorFactory::Create() {
    auto signaling_server = std::make_shared<core_signaling::SignalingClient>(signaling_config_->GetSignalingUrl());
    signaling_server->Start();

    auto client_state_manager = std::make_shared<ConnectorStateManager>(signaling_server);

    auto mosaic_connector = std::make_shared<MosaicConnector>(robot_id_, user_id_, client_state_manager);
    signaling_server->SetGCSConnector(mosaic_connector);

    const auto peer_connection_manager = std::make_shared<PeerConnectionManager>(
        mosaic_connector, mosaic_connector->GetStateManager(), signaling_server, ice_config_);
    mosaic_connector->SetPeerConnectionManager(peer_connection_manager);

    mosaic_connector->InitializeWebRTC();

    return mosaic_connector;
}
