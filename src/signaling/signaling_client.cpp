//
// Created by yhkim on 25. 6. 2.
//

#include <api/jsep.h>
#include <json/json.h>
#include <mosaic/logger/log.h>
#include <mosaic/signaling/signaling_client.h>

using namespace mosaic::core_signaling;

webrtc::IceCandidateInterface* ResolveIceCandidate(Json::Value const& ice_candidate);

webrtc::SessionDescriptionInterface* ResolveSdpOffer(std::string const& sdp_offer);

SignalingClient::SignalingClient(const std::string& ws_uri) : ws_uri_(ws_uri) {}

SignalingClient::~SignalingClient() {
    SignalingClient::Stop();
}

void SignalingClient::Start() {
    StartInternal();
}

void SignalingClient::Stop() {
    if (is_connected_) {
        ws_client_->disconnect();
        is_connected_ = false;
    }
}

void SignalingClient::SendSdpAnswer(const webrtc::SessionDescriptionInterface* sdp) const {
    if (!is_connected_ || current_rtc_connection_id_.empty())
        return;

    std::string sdp_str;
    sdp->ToString(&sdp_str);

    Json::Value message;
    message["type"] = "signaling.send_sdp_answer";
    Json::Value data;
    data["rtcConnectionId"] = current_rtc_connection_id_;
    data["sdpAnswer"] = sdp_str;

    message["data"] = data;
    SendWsMessage(message);
}

void SignalingClient::SendIceCandidate(const webrtc::IceCandidateInterface* candidate) const {
    if (!is_connected_ || current_rtc_connection_id_.empty())
        return;

    std::string candidate_str;
    candidate->ToString(&candidate_str);

    Json::Value message;
    message["type"] = "signaling.send_ice_candidate";
    Json::Value data;
    data["rtcConnectionId"] = current_rtc_connection_id_;

    Json::Value ice_candidate;
    ice_candidate["candidate"] = candidate_str;
    ice_candidate["sdpMid"] = candidate->sdp_mid();
    ice_candidate["sdpMLineIndex"] = candidate->sdp_mline_index();

    data["iceCandidate"] = ice_candidate;
    message["data"] = data;

    SendWsMessage(message);
}

void SignalingClient::SendState(const int state) const {
    const_cast<SignalingClient*>(this)->latest_state_ = state;

    if (!is_connected_)
        return;

    Json::Value message;
    message["type"] = "status.update";
    Json::Value data;
    data["status"] = latest_state_;
    message["data"] = data;

    SendWsMessage(message);
}

void SignalingClient::SetMosaicConnector(const std::shared_ptr<core::MosaicConnector>& mosaic_connector) {
    mosaic_connector_ = mosaic_connector;
}

void SignalingClient::SetAuthenticator(const std::shared_ptr<security::IMosaicAuthenticator>& authenticator) {
    authenticator_ = authenticator;
}

bool SignalingClient::IsAuthenticated() const {
    return authenticator_->IsAuthenticated();
}

void SignalingClient::StartInternal() {
    ws_client_ = std::make_unique<WebSocketClient>();

    ws_client_->setOnConnected([this]() {
        MOSAIC_LOG_INFO("Connected to WebSocket server.");
        is_connected_ = true;
        if (latest_state_ != -1)
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

void SignalingClient::OnWsMessage(const std::string& msg) const {
    Json::Value message;
    const Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    std::string errors;

    const bool parsingSuccessful = reader->parse(msg.c_str(), msg.c_str() + msg.size(), &message, &errors);
    delete reader;

    if (!parsingSuccessful) {
        MOSAIC_LOG_ERROR("JSON parse error: {}", errors);
        MOSAIC_LOG_ERROR("Original Message: {}", msg);
        return;
    }
    OnMessage(message);
}

void SignalingClient::OnMessage(Json::Value const& message) const {
    if (!message["type"]) {
        return;
    }

    if (const std::string type = message["type"].asString(); type == "signaling.send_sdp_offer") {
        HandleSendSdpOffer(message["data"]);
    } else if (type == "signaling.exchange_ice_candidate") {
        HandleExchangeIceCandidate(message["data"]);
    } else if (type == "signaling.close_peer_connection") {
        HandleCloseConnection();
    } else if (type.rfind("authorize", 0) == 0) {
        HandleAuthorizationMessage(type, message["data"]);
    } else {
        MOSAIC_LOG_DEBUG("Received unknown message type: {}", type);
    }
}

void SignalingClient::SendWsMessage(const Json::Value& message) const {
    if (ws_client_->isConnected()) {
        ws_client_->sendJson(message);
    }
}

void SignalingClient::HandleSendSdpOffer(const Json::Value& message) const {
    if (!message["sdpOffer"]) {
        MOSAIC_LOG_WARNING("Received null SDP offer; ignoring.");
        return;
    }
    // Process the SDP offer...
    if (const auto sdp_offer = ResolveSdpOffer(message["sdpOffer"].asString())) {
        mosaic_connector_->HandleSdpOffer(sdp_offer);
    }
}

void SignalingClient::HandleExchangeIceCandidate(Json::Value const& message) const {
    if (!message["iceCandidate"]) {
        MOSAIC_LOG_WARNING("Received null ICE candidate; ignoring.");
        return;
    }

    if (const auto candidate = ResolveIceCandidate(message["iceCandidate"])) {
        // Process the ICE candidate...
        mosaic_connector_->HandleIceCandidateFromSignaling(candidate);
    }
}

void SignalingClient::HandleCloseConnection() const {
    mosaic_connector_->ClosePeerConnection();
}

void SignalingClient::HandleAuthorizationMessage(const std::string& type, Json::Value const& message) const {
    if (type == "authorize.req") {
        authenticator_->HandleAuthenticationReq();
    } else if (type == "authorize.res") {
        if (message && message.asInt() == 100) {
            authenticator_->HandleAuthenticationComplete();
        }
    } else {
        authenticator_->HandleAuthenticationMessage(message);
    }
}

webrtc::IceCandidateInterface* ResolveIceCandidate(Json::Value const& ice_candidate) {
    MOSAIC_LOG_DEBUG("Received ICE candidate: {}", ice_candidate["candidate"].asString());
    if (!ice_candidate["candidate"] || !ice_candidate["sdpMid"] || !ice_candidate["sdpMLineIndex"]) {
        MOSAIC_LOG_WARNING("Received null ICE candidate string; ignoring.");
        return nullptr;
    }
    std::string candidate_str = ice_candidate["candidate"].asString();
    const std::string sdp_mid = ice_candidate["sdpMid"].asString();
    const int sdp_mline_index = ice_candidate["sdpMLineIndex"].asInt();

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
    MOSAIC_LOG_DEBUG("Received Sdp Offer: {}", sdp_offer);
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
