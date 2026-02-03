//
// Created by yhkim on 25. 7. 7.
//

#include <mosaic/core/mosaic_connector_factory.h>
#include <mosaic/signaling/signaling_client.h>

#include "mosaic/security/authenticator_resolver.h"

using namespace mosaic::core;

std::shared_ptr<MosaicConnector> MosaicConnectorFactory::Create() const {
    auto signaling_client = std::make_shared<SignalingClient>(server_config_->GetSignalingUrl());

    auto authenticator = security::AuthenticatorResolver::GetInstance().CreateAuthenticator(server_config_->auth_config,
                                                                                            signaling_client);
    if (!authenticator) {
        MOSAIC_LOG_ERROR("Failed to create authenticator.");
        return nullptr;
    }
    signaling_client->SetAuthenticator(authenticator);

    signaling_client->Start();

    auto client_state_manager = std::make_shared<ConnectorStateManager>(signaling_client);

    auto mosaic_connector = std::make_shared<MosaicConnector>(client_state_manager);
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
