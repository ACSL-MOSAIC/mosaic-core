//
// Created by yhkim on 1/30/26.
//

#ifndef MOSAIC_CORE_A_MOSAIC_AUTHENTICATOR_HPP
#define MOSAIC_CORE_A_MOSAIC_AUTHENTICATOR_HPP

#include <memory>
#include <utility>

#include <json/json.h>

#include "../configs_decl.hpp"
#include "auth_type.hpp"

namespace mosaic::core_signaling {
class ISignalingClient;
}

namespace mosaic::security {
class IMosaicAuthenticator {
  public:
    IMosaicAuthenticator() = default;

    virtual ~IMosaicAuthenticator() = default;

    virtual AuthType GetAuthType() = 0;

    virtual std::string GetRobotId() = 0;

    virtual bool IsAuthenticated() = 0;

    virtual void HandleAuthenticationReq() = 0;

    virtual void HandleAuthenticationMessage(Json::Value message) = 0;

    virtual void HandleAuthenticationComplete() = 0;
};

class AMosaicAuthenticator : public IMosaicAuthenticator {
  public:
    explicit AMosaicAuthenticator(core::AuthConfig auth_config,
                                  const std::shared_ptr<core_signaling::ISignalingClient>& signaling_client)
        : auth_config_(std::move(auth_config)), signaling_client_(signaling_client) {}

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

#endif  // MOSAIC_CORE_A_MOSAIC_AUTHENTICATOR_HPP
