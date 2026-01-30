//
// Created by yhkim on 1/30/26.
//

#ifndef MOSAIC_CORE_A_MOSAIC_AUTHENTICATOR_H
#define MOSAIC_CORE_A_MOSAIC_AUTHENTICATOR_H

#include "i_mosaic_authenticator.h"

namespace mosaic::security {
class AMosaicAuthenticator : public IMosaicAuthenticator {
  public:
    explicit AMosaicAuthenticator(const core::AuthConfig& auth_config,
                                  const std::shared_ptr<core_signaling::ISignalingClient>& signaling_client)
        : IMosaicAuthenticator(auth_config, signaling_client),
          auth_config_(auth_config),
          signaling_client_(signaling_client) {}

    ~AMosaicAuthenticator() override = default;

    AuthType GetAuthType() override = 0;

    std::string GetRobotId() override {
        return auth_config_.robot_id;
    }

    bool IsAuthenticated() override {
        return authenticated_;
    }

    virtual void HandleAuthenticationMessage(Json::Value message) override;

    void HandleAuthenticationComplete() override;

  protected:
    bool authenticated_ = false;
    core::AuthConfig auth_config_;
    std::shared_ptr<core_signaling::ISignalingClient> signaling_client_;
};
}  // namespace mosaic::security

#endif  // MOSAIC_CORE_A_MOSAIC_AUTHENTICATOR_H
