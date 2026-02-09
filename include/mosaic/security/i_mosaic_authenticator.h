//
// Created by yhkim on 12/25/25.
//

#ifndef MOSAIC_SECURITY_I_MOSAIC_AUTHENTICATOR_H
#define MOSAIC_SECURITY_I_MOSAIC_AUTHENTICATOR_H

#include <memory>

#include "auth_type.h"
#include "mosaic/configs_decl.h"
#include "mosaic/json_forward_decl.h"
namespace mosaic::core_signaling {
class ISignalingClient;
}

namespace mosaic::security {
class IMosaicAuthenticator {
  public:
    explicit IMosaicAuthenticator(const core::AuthConfig& auth_config,
                                  const std::shared_ptr<core_signaling::ISignalingClient>& signaling_client) {}

    virtual ~IMosaicAuthenticator() = default;

    virtual AuthType GetAuthType() = 0;

    virtual std::string GetRobotId() = 0;

    virtual bool IsAuthenticated() = 0;

    virtual void HandleAuthenticationReq() = 0;

    virtual void HandleAuthenticationMessage(Json::Value message) = 0;

    virtual void HandleAuthenticationComplete() = 0;
};
}  // namespace mosaic::security

#endif  // MOSAIC_SECURITY_I_MOSAIC_AUTHENTICATOR_H
