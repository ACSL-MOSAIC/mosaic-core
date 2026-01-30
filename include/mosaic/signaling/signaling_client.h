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

namespace mosaic::core_signaling {
class SignalingClient : public ISignalingClient {
  public:
    explicit SignalingClient(const std::string& ws_uri);

    ~SignalingClient() override;

    void Start() override;

    void Stop() override;

    void SendSdpAnswer(const webrtc::SessionDescriptionInterface* sdp) const override;

    void SendIceCandidate(const webrtc::IceCandidateInterface* candidate) const override;

    void SendState(const int state) const override;

    void SetMosaicConnector(const std::shared_ptr<core::MosaicConnector>& mosaic_connector) override;

    void SetAuthenticator(const std::shared_ptr<security::IMosaicAuthenticator>& authenticator) override;

    bool IsAuthenticated() const override;

    void SendWsMessage(const Json::Value& message) const override;

  private:
    void OnWsMessage(const std::string& msg) const;

    void OnMessage(Json::Value const& message) const;

    void HandleSendSdpOffer(Json::Value const& message) const;

    void HandleExchangeIceCandidate(Json::Value const& message) const;

    void HandleCloseConnection() const;

    void HandleAuthorizationMessage(const std::string& type, Json::Value const& message) const;

    std::shared_ptr<core::MosaicConnector> mosaic_connector_;
    std::shared_ptr<security::IMosaicAuthenticator> authenticator_;
    bool is_connected_ = false;
    std::string ws_uri_;
    std::unique_ptr<WebSocketClient> ws_client_ = nullptr;
    int latest_state_ = -1;
    std::string current_rtc_connection_id_;

    void StartInternal();
};
}  // namespace mosaic::core_signaling

#endif  // MOSAIC_RTC_CORE_SIGNALING_CLIENT_H
