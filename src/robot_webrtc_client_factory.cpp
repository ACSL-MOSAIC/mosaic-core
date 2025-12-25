//
// Created by yhkim on 25. 7. 7.
//
#include "rtc_sender/robot_webrtc_client_factory.h"

#include "rtc_sender/ice_config.h"
#include "rtc_sender/signaling_client.h"

using namespace rtc_sender;

RobotWebRTCClientFactory::RobotWebRTCClientFactory(const std::string &robot_id,
                                                   const std::string &user_id,
                                                   const std::shared_ptr<SignalingConfig> &signaling_config,
                                                   const std::shared_ptr<IceConfig> &ice_config) {
    robot_id_ = robot_id;
    user_id_ = user_id;
    signaling_config_ = signaling_config;
    ice_config_ = ice_config;
}

RobotWebRTCClientFactory::RobotWebRTCClientFactory(const std::string &robot_id,
                                                   const std::string &user_id,
                                                   const std::string &ws_uri,
                                                   const std::string &turn_url,
                                                   const std::string &turn_username,
                                                   const std::string &turn_credential) {
    robot_id_ = robot_id;
    user_id_ = user_id;
    signaling_config_ = std::make_shared<SignalingConfig>(ws_uri, robot_id);
    ice_config_ = std::make_shared<IceConfig>(turn_url, turn_username, turn_credential);
}

std::shared_ptr<GCSConnector> RobotWebRTCClientFactory::Create() {
    auto signaling_server = std::make_shared<SignalingClient>(signaling_config_->GetSignalingUrl());
    signaling_server->Start();

    auto client_state_manager = std::make_shared<ClientStateManager>(signaling_server);

    auto webrtc_client = std::make_shared<GCSConnector>(robot_id_, user_id_, client_state_manager);
    signaling_server->SetRobotWebRTCClient(webrtc_client);

    auto peer_connection_manager = std::make_shared<PeerConnectionManager>(
        webrtc_client, webrtc_client->GetStateManager(), signaling_server, ice_config_);
    webrtc_client->SetPeerConnectionManager(peer_connection_manager);

    webrtc_client->InitializeWebRTC();

    return webrtc_client;
}
