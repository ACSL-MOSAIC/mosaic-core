//
// Created by yhkim on 1/30/26.
//

#ifndef MOSAIC_CORE_SIMPLE_TOKEN_AUTHENTICATOR_H
#define MOSAIC_CORE_SIMPLE_TOKEN_AUTHENTICATOR_H

#include "mosaic/security/a_mosaic_authenticator.h"

namespace mosaic::security {
class SimpleTokenAuthenticator : public AMosaicAuthenticator {
  public:
    explicit SimpleTokenAuthenticator(const core::AuthConfig& auth_config,
                                      const std::shared_ptr<core_signaling::ISignalingClient>& signaling_client)
        : AMosaicAuthenticator(auth_config, signaling_client) {}

    ~SimpleTokenAuthenticator() override = default;

    AuthType GetAuthType() override {
        return AuthTypeConst::SIMPLE_TOKEN;
    }

    void HandleAuthenticationReq() override;
};
}  // namespace mosaic::security

#endif  // MOSAIC_CORE_SIMPLE_TOKEN_AUTHENTICATOR_H
