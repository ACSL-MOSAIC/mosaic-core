//
// Created by yhkim on 1/30/26.
//

#include "mosaic/security/authenticator_resolver.h"

#include <iostream>

#include "mosaic/logger/log.h"
#include "mosaic/security/impl/no_auth_authenticator.h"
#include "mosaic/security/impl/simple_token_authenticator.h"

using namespace mosaic::security;

AuthenticatorResolver AuthenticatorResolver::instance_;

AuthenticatorResolver& AuthenticatorResolver::GetInstance() {
    return instance_;
}

std::shared_ptr<IMosaicAuthenticator> AuthenticatorResolver::CreateAuthenticator(
    const core::AuthConfig& auth_config,
    const std::shared_ptr<core_signaling::ISignalingClient>& signaling_client) {
    auto it = factory_map_.find(auth_config.type);
    if (it != factory_map_.end()) {
        return it->second->CreateAuthenticator(auth_config, signaling_client);
    }
    return nullptr;
}

AuthenticatorResolver::AuthenticatorResolver() {
    // Register authenticator factories
    factory_map_[AuthTypeConst::NO_AUTHORIZATION] = std::make_shared<AuthenticatorFactory<NoAuthAuthenticator>>();
    factory_map_[AuthTypeConst::SIMPLE_TOKEN] = std::make_shared<AuthenticatorFactory<SimpleTokenAuthenticator>>();
}
