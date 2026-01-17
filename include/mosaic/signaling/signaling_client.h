//
// Created by yhkim on 25. 6. 2.
//

#ifndef MOSAIC_RTC_CORE_SIGNALING_CLIENT_H
#define MOSAIC_RTC_CORE_SIGNALING_CLIENT_H

#include <memory>
#include <string>

#include <mosaic/core/mosaic_connector.h>
#include <mosaic/signaling/i_signaling_client.h>
#include <mosaic/signaling/websocket_client.h>

// forward declare
namespace Json {
class Value;
}  // namespace Json

namespace mosaic::core_signaling {
class SignalingClient : public ISignalingClient {
  public:
    explicit SignalingClient(const std::string& ws_uri);

    ~SignalingClient() override;

    void Start() override;

    void Stop() override;

    void SendSdpAnswer(const webrtc::SessionDescriptionInterface* sdp) const override;

    void SendIceCandidate(const webrtc::IceCandidateInterface* candidate) const override;

    void SendState(const std::string& state) const override;

    void SetMosaicConnector(const std::shared_ptr<core::MosaicConnector>& gcs_connector) const override;

    void SetAuthenticator(const std::shared_ptr<security::IMosaicAuthenticator>& authenticator) const override;

    [[nodiscard]] bool IsAuthenticated() const override;

  private:
    void OnWsMessage(const std::string& msg) const;

    void OnMessage(Json::Value const& message) const;

    void SendWsMessage(const Json::Value& message) const;

    std::shared_ptr<core::MosaicConnector> mosaic_connector_;
    std::shared_ptr<security::IMosaicAuthenticator> authenticator_;
    bool is_connected_ = false;
    std::string ws_uri_;
    std::unique_ptr<WebSocketClient> ws_client_ = nullptr;
    std::string latest_state_;

    void StartInternal();
};
}  // namespace mosaic::core_signaling

#endif  // MOSAIC_RTC_CORE_SIGNALING_CLIENT_H
