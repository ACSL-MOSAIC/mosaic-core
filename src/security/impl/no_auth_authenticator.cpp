//
// Created by yhkim on 1/30/26.
//

#include "mosaic/security/impl/no_auth_authenticator.hpp"

#include <json/json.h>

#include "mosaic/signaling/i_signaling_client.hpp"

using namespace mosaic::security;

void NoAuthAuthenticator::HandleAuthenticationReq() {
    Json::Value message;
    message["type"] = "authorize";

    Json::Value data;
    data["robotId"] = auth_config_.robot_id;
    message["data"] = data;

    signaling_client_->SendWsMessage(message);
}
