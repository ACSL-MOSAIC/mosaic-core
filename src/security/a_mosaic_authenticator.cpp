//
// Created by yhkim on 1/30/26.
//
#include "mosaic/security/a_mosaic_authenticator.h"

#include <json/value.h>

#include "mosaic/logger/log.h"

using namespace mosaic::security;

void AMosaicAuthenticator::HandleAuthenticationMessage(Json::Value message) {
    // Nothing to do
    MOSAIC_LOG_INFO("Authentication related message received, but no action is taken for auth type: {}", GetAuthType());
}

void AMosaicAuthenticator::HandleAuthenticationComplete() {
    authenticated_ = true;
}