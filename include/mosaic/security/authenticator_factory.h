//
// Created by yhkim on 1/30/26.
//

#ifndef MOSAIC_CORE_AUTHENTICATOR_FACTORY_H
#define MOSAIC_CORE_AUTHENTICATOR_FACTORY_H
#include <memory>

#include "i_mosaic_authenticator.h"
#include "mosaic/configs_decl.h"

namespace mosaic::security {
class IAuthenticatorFactory {
  public:
    virtual ~IAuthenticatorFactory() = default;
    virtual std::shared_ptr<IMosaicAuthenticator> CreateAuthenticator(
        const core::AuthConfig& auth_config,
        const std::shared_ptr<core_signaling::ISignalingClient>& signaling_client) = 0;
};

template <typename T>
class AuthenticatorFactory : public IAuthenticatorFactory {
    static_assert(std::is_base_of_v<IMosaicAuthenticator, T>, "T must inherit from IMosaicAuthenticator");

  public:
    AuthenticatorFactory() = default;

    std::shared_ptr<IMosaicAuthenticator> CreateAuthenticator(
        const core::AuthConfig& auth_config,
        const std::shared_ptr<core_signaling::ISignalingClient>& signaling_client) override {
        return std::make_shared<T>(auth_config, signaling_client);
    }
};
}  // namespace mosaic::security

#endif  // MOSAIC_CORE_AUTHENTICATOR_FACTORY_H
