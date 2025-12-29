//
// Created by yhkim on 25. 6. 2.
//

#include <utility>

#include <api/jsep.h>
#include <mosaic_rtc_core/logger/log.h>
#include <mosaic_rtc_core/signaling/signaling_client.h>
#include <mosaic_rtc_core/signaling/websocket_client.h>

using namespace mosaic::core_signaling;

webrtc::IceCandidateInterface* ResolveIceCandidate(json::value const& ice_candidate);

webrtc::SessionDescriptionInterface* ResolveSdpOffer(std::string const& sdp_offer);

class SignalingClient::Impl {
  public:
    explicit Impl(std::string ws_uri) : ws_uri_(std::move(ws_uri)) {}

    void Start() {
        StartInternal();
    }

    void Stop() {
        if (is_connected_) {
            ws_client_->disconnect();
            is_connected_ = false;
        }
    }

    void SendSdpAnswer(const webrtc::SessionDescriptionInterface* sdp) const {
        if (!is_connected_)
            return;

        std::string sdp_str;
        sdp->ToString(&sdp_str);

        json::value message;
        message["type"] = json::value::string("send_sdp_answer");
        message["robot_id"] = json::value::string(robot_webrtc_client_->GetRobotId());
        message["user_id"] = json::value::string(robot_webrtc_client_->GetUserId());
        message["sdp_answer"] = json::value::string(sdp_str);

        SendWsMessage(message);
    }

    void SendIceCandidate(const webrtc::IceCandidateInterface* candidate) const {
        if (!is_connected_)
            return;

        std::string candidate_str;
        candidate->ToString(&candidate_str);

        json::value message;
        message["type"] = json::value::string("send_ice_candidate");
        message["robot_id"] = json::value::string(robot_webrtc_client_->GetRobotId());
        message["user_id"] = json::value::string(robot_webrtc_client_->GetUserId());

        json::value ice_candidate;
        ice_candidate["candidate"] = json::value::string(candidate_str);
        ice_candidate["sdpMid"] = json::value::string(candidate->sdp_mid());
        ice_candidate["sdpMLineIndex"] = candidate->sdp_mline_index();

        message["ice_candidate"] = ice_candidate;

        SendWsMessage(message);
    }

    void SendState(const std::string& state) {
        latest_state_ = state;

        if (!is_connected_)
            return;

        json::value message;
        message["type"] = json::value::string("send_state");
        message["robot_id"] = json::value::string(robot_webrtc_client_->GetRobotId());
        message["user_id"] = json::value::string(robot_webrtc_client_->GetUserId());
        message["state"] = json::value::string(latest_state_);

        SendWsMessage(message);
    }

    void SetGCSConnector(const std::shared_ptr<core::MosaicConnector>& robot_webrtc_client) {
        robot_webrtc_client_ = robot_webrtc_client;
    }

    void SetAuthenticator(const std::shared_ptr<security::IGCSAuthenticator>& authenticator) {
        authenticator_ = authenticator;
    }

    [[nodiscard]] bool IsAuthenticated() const {
        return authenticator_->IsAuthenticated();
    }

  private:
    void StartInternal() {
        ws_client_ = std::make_unique<WebSocketClient>();

        ws_client_->setOnConnected([this]() {
            MOSAIC_LOG_INFO("Connected to WebSocket server.");
            is_connected_ = true;
            if (!latest_state_.empty())
                SendState(latest_state_);
        });

        ws_client_->setOnDisconnected([this]() {
            MOSAIC_LOG_INFO("WebSocket connection disconnected.");
            is_connected_ = false;
        });

        ws_client_->setOnError([this](const std::string& error) {
            MOSAIC_LOG_ERROR("WebSocket connection failed: {}", error);
            is_connected_ = false;
        });

        ws_client_->setOnMessage([this](const std::string& message) {
            MOSAIC_LOG_INFO("WS Message Received: {}", message);
            OnWsMessage(message);
        });

        MOSAIC_LOG_INFO("Signaling Server connecting to {}", ws_uri_);

        ws_client_->connect(ws_uri_);
        if (!ws_client_->isConnected()) {
            MOSAIC_LOG_ERROR("Failed to connect to WebSocket server at {}", ws_uri_);
        }
    }

    void OnWsMessage(const std::string& msg) const {
        json::value message;
        try {
            message = json::value::parse(msg);
        } catch (const std::exception& e) {
            MOSAIC_LOG_ERROR("JSON parse error: {}", e.what());
            MOSAIC_LOG_ERROR("Original Message: {}", msg);
            return;
        }
        OnMessage(message);
    }

    void OnMessage(json::value const& message) const {
        if (!message.has_string_field("type")) {
            return;
        }
        std::string type = message.at("type").as_string();

        if (type == "receive_sdp_offer") {
            // Handle SDP offer
            if (!message.has_string_field("sdp_offer")) {
                MOSAIC_LOG_WARNING("Received null SDP offer; ignoring.");
                return;
            }
            const std::string sdp_offer_str = message.at("sdp_offer").as_string();
            MOSAIC_LOG_DEBUG("Received SDP offer: {}", sdp_offer_str);

            // Process the SDP offer...
            if (const auto sdp_offer = ResolveSdpOffer(sdp_offer_str)) {
                robot_webrtc_client_->HandleSdpOffer(sdp_offer);
            }
        } else if (type == "receive_ice_candidate") {
            if (!message.has_object_field("ice_candidate")) {
                MOSAIC_LOG_WARNING("Received null ICE candidate; ignoring.");
                return;
            }
            auto ice_candidate = message.at("ice_candidate");
            if (!ice_candidate.has_string_field("candidate") || ice_candidate.at("candidate").as_string().empty() ||
                !ice_candidate.has_string_field("sdpMid") || !ice_candidate.has_integer_field("sdpMLineIndex")) {
                MOSAIC_LOG_WARNING("Received null ICE candidate string; ignoring.");
                return;
            }

            MOSAIC_LOG_DEBUG("Received ICE candidate: {}", ice_candidate.at("candidate").as_string());
            MOSAIC_LOG_DEBUG("SdpMid: {}, SdpMLineIndex: {}",
                             ice_candidate.at("sdpMid").as_string(),
                             ice_candidate.at("sdpMLineIndex").as_integer());

            if (const auto candidate = ResolveIceCandidate(message.at("ice_candidate"))) {
                // Process the ICE candidate...
                robot_webrtc_client_->HandleIceCandidateFromSignaling(candidate);
            }
        } else if (type == "send_close_peer_connection") {
            robot_webrtc_client_->ClosePeerConnection();
        } else {
            MOSAIC_LOG_DEBUG("Received unknown message type: {}", type);
        }
    }

    void SendWsMessage(const json::value& message) const {
        if (ws_client_->isConnected()) {
            ws_client_->sendJson(message);
        }
    }

    std::shared_ptr<core::MosaicConnector> robot_webrtc_client_;
    std::shared_ptr<security::IGCSAuthenticator> authenticator_;
    bool is_connected_ = false;
    std::string ws_uri_;
    std::unique_ptr<WebSocketClient> ws_client_ = nullptr;
    std::string latest_state_;
};

SignalingClient::SignalingClient(const std::string& ws_uri) : pImpl(std::make_unique<Impl>(ws_uri)) {}

SignalingClient::~SignalingClient() {
    pImpl->Stop();
}

void SignalingClient::Start() {
    pImpl->Start();
}

void SignalingClient::Stop() {
    pImpl->Stop();
}

void SignalingClient::SendSdpAnswer(const webrtc::SessionDescriptionInterface* sdp) const {
    pImpl->SendSdpAnswer(sdp);
}

void SignalingClient::SendIceCandidate(const webrtc::IceCandidateInterface* candidate) const {
    pImpl->SendIceCandidate(candidate);
}

void SignalingClient::SendState(const std::string& state) const {
    pImpl->SendState(state);
}

void SignalingClient::SetGCSConnector(const std::shared_ptr<core::MosaicConnector>& gcs_connector) const {
    pImpl->SetGCSConnector(gcs_connector);
}

void SignalingClient::SetAuthenticator(const std::shared_ptr<security::IGCSAuthenticator>& authenticator) const {
    pImpl->SetAuthenticator(authenticator);
}

bool SignalingClient::IsAuthenticated() const {
    return pImpl->IsAuthenticated();
}

webrtc::IceCandidateInterface* ResolveIceCandidate(json::value const& ice_candidate) {
    std::string candidate_str = ice_candidate.at("candidate").as_string();
    const std::string sdp_mid = ice_candidate.at("sdpMid").as_string();
    const int sdp_mline_index = ice_candidate.at("sdpMLineIndex").as_integer();

    webrtc::SdpParseError error;
    webrtc::IceCandidateInterface* candidate =
        webrtc::CreateIceCandidate(sdp_mid, sdp_mline_index, candidate_str, &error);
    if (!candidate) {
        MOSAIC_LOG_ERROR("Failed to create IceCandidate from string: {}, error: {}, from line {}",
                         candidate_str,
                         error.description,
                         error.line);
        return nullptr;
    }

    return candidate;
}

webrtc::SessionDescriptionInterface* ResolveSdpOffer(std::string const& sdp_offer) {
    webrtc::SdpParseError error;
    std::unique_ptr<webrtc::SessionDescriptionInterface> sdp =
        webrtc::CreateSessionDescription(webrtc::SdpType::kOffer, sdp_offer, &error);
    if (!sdp) {
        MOSAIC_LOG_ERROR("Failed to create SessionDescription from SDP string: {}, error: {}, from line {}",
                         sdp_offer,
                         error.description,
                         error.line);
        // TODO handle error
        return nullptr;
    }

    return sdp.release();
}
