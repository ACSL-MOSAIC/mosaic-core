//
// Created by yhkim on 25. 7. 7.
//

#include <mosaic/core/mosaic_connector_factory.h>
#include <mosaic/signaling/signaling_client.h>

using namespace mosaic::core;

std::shared_ptr<MosaicConnector> MosaicConnectorFactory::Create() const {
    auto signaling_client = std::make_shared<core_signaling::SignalingClient>(server_config_->GetSignalingUrl());
    signaling_client->Start();

    auto client_state_manager = std::make_shared<ConnectorStateManager>(signaling_client);

    auto mosaic_connector = std::make_shared<MosaicConnector>(
        server_config_->auth_config.robot_id, server_config_->auth_config.user_id, client_state_manager);
    signaling_client->SetMosaicConnector(mosaic_connector);

    const auto peer_connection_manager =
        std::make_shared<PeerConnectionManager>(mosaic_connector,
                                                mosaic_connector->GetStateManager(),
                                                signaling_client,
                                                std::make_shared<WebRtcConfig>(server_config_->webrtc_config));
    mosaic_connector->SetPeerConnectionManager(peer_connection_manager);

    mosaic_connector->InitializeWebRTC();

    return mosaic_connector;
}
