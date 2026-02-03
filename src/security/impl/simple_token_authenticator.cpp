//
// Created by yhkim on 1/30/26.
//

#include "mosaic/security/impl/simple_token_authenticator.h"

#include <json/value.h>

#include "mosaic/signaling/i_signaling_client.h"

using namespace mosaic::security;

void SimpleTokenAuthenticator::HandleAuthenticationReq() {
    Json::Value message;
    message["type"] = "authorize";

    Json::Value data;
    data["robotId"] = auth_config_.robot_id;
    data["data"] = auth_config_.params["token"];
    message["data"] = data;

    signaling_client_->SendWsMessage(message);
}