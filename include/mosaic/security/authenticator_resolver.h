//
// Created by yhkim on 1/30/26.
//

#ifndef MOSAIC_CORE_AUTHENTICATOR_RESOLVER_H
#define MOSAIC_CORE_AUTHENTICATOR_RESOLVER_H

#include <memory>
#include <unordered_map>

#include "auth_type.h"
#include "authenticator_factory.h"
#include "i_mosaic_authenticator.h"
#include "mosaic/configs_decl.h"

namespace mosaic::security {
class AuthenticatorResolver {
  public:
    static AuthenticatorResolver& GetInstance() {
        static AuthenticatorResolver instance;
        return instance;
    }

    std::shared_ptr<IMosaicAuthenticator> CreateAuthenticator(
        const core::AuthConfig& auth_config,
        const std::shared_ptr<core_signaling::ISignalingClient>& signaling_client);

    AuthenticatorResolver(const AuthenticatorResolver&) = delete;

    AuthenticatorResolver& operator=(const AuthenticatorResolver&) = delete;

  private:
    std::unordered_map<AuthType, std::shared_ptr<IAuthenticatorFactory>> factory_map_;

    AuthenticatorResolver();

    ~AuthenticatorResolver() = default;
};
}  // namespace mosaic::security

#endif  // MOSAIC_CORE_AUTHENTICATOR_RESOLVER_H
