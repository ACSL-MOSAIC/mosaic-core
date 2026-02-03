//
// Created by yhkim on 1/30/26.
//

#ifndef MOSAIC_CORE_NO_AUTH_AUTHENTICATOR_H
#define MOSAIC_CORE_NO_AUTH_AUTHENTICATOR_H

#include <memory>

#include "mosaic/security/a_mosaic_authenticator.h"

namespace mosaic::security {
class NoAuthAuthenticator : public AMosaicAuthenticator {
  public:
    explicit NoAuthAuthenticator(const core::AuthConfig& auth_config,
                                 const std::shared_ptr<core_signaling::ISignalingClient>& signaling_client)
        : AMosaicAuthenticator(auth_config, signaling_client) {}

    ~NoAuthAuthenticator() override = default;

    AuthType GetAuthType() override {
        return AuthTypeConst::NO_AUTHORIZATION;
    }

    void HandleAuthenticationReq() override;
};
}  // namespace mosaic::security

#endif  // MOSAIC_CORE_NO_AUTH_AUTHENTICATOR_H
