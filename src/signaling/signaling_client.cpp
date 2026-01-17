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
    if (!is_connected_)
        return;

    std::string sdp_str;
    sdp->ToString(&sdp_str);

    Json::Value message;
    message["type"] = "send_sdp_answer";
    message["robot_id"] = mosaic_connector_->GetRobotId();
    message["user_id"] = mosaic_connector_->GetUserId();
    message["sdp_answer"] = sdp_str;

    SendWsMessage(message);
}

void SignalingClient::SendIceCandidate(const webrtc::IceCandidateInterface* candidate) const {
    if (!is_connected_)
        return;

    std::string candidate_str;
    candidate->ToString(&candidate_str);

    Json::Value message;
    message["type"] = "send_ice_candidate";
    message["robot_id"] = mosaic_connector_->GetRobotId();
    message["user_id"] = mosaic_connector_->GetUserId();

    Json::Value ice_candidate;
    ice_candidate["candidate"] = candidate_str;
    ice_candidate["sdpMid"] = candidate->sdp_mid();
    ice_candidate["sdpMLineIndex"] = candidate->sdp_mline_index();

    message["ice_candidate"] = ice_candidate;

    SendWsMessage(message);
}

void SignalingClient::SendState(const std::string& state) const {
    const_cast<SignalingClient*>(this)->latest_state_ = state;

    if (!is_connected_)
        return;

    Json::Value message;
    message["type"] = "send_state";
    message["robot_id"] = mosaic_connector_->GetRobotId();
    message["user_id"] = mosaic_connector_->GetUserId();
    message["state"] = latest_state_;

    SendWsMessage(message);
}

void SignalingClient::SetMosaicConnector(const std::shared_ptr<core::MosaicConnector>& gcs_connector) const {
    const_cast<SignalingClient*>(this)->mosaic_connector_ = gcs_connector;
}

void SignalingClient::SetAuthenticator(const std::shared_ptr<security::IMosaicAuthenticator>& authenticator) const {
    const_cast<SignalingClient*>(this)->authenticator_ = authenticator;
}

bool SignalingClient::IsAuthenticated() const {
    return authenticator_->IsAuthenticated();
}

void SignalingClient::StartInternal() {
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

    if (const std::string type = message["type"].asString(); type == "receive_sdp_offer") {
        // Handle SDP offer
        if (!message["sdp_offer"]) {
            MOSAIC_LOG_WARNING("Received null SDP offer; ignoring.");
            return;
        }
        const std::string sdp_offer_str = message["sdp_offer"].asString();

        // Process the SDP offer...
        if (const auto sdp_offer = ResolveSdpOffer(sdp_offer_str)) {
            mosaic_connector_->HandleSdpOffer(sdp_offer);
        }
    } else if (type == "receive_ice_candidate") {
        if (!message["ice_candidate"]) {
            MOSAIC_LOG_WARNING("Received null ICE candidate; ignoring.");
            return;
        }
        auto ice_candidate = message["ice_candidate"];
        if (!ice_candidate["candidate"] || ice_candidate["candidate"].asString().empty() || !ice_candidate["sdpMid"] ||
            !ice_candidate["sdpMLineIndex"]) {
            MOSAIC_LOG_WARNING("Received null ICE candidate string; ignoring.");
            return;
        }

        MOSAIC_LOG_DEBUG("Received ICE candidate: {}", ice_candidate["candidate"].asString());
        MOSAIC_LOG_DEBUG("SdpMid: {}, SdpMLineIndex: {}",
                         ice_candidate["sdpMid"].asString(),
                         ice_candidate["sdpMLineIndex"].asInt());

        if (const auto candidate = ResolveIceCandidate(message["ice_candidate"])) {
            // Process the ICE candidate...
            mosaic_connector_->HandleIceCandidateFromSignaling(candidate);
        }
    } else if (type == "send_close_peer_connection") {
        mosaic_connector_->ClosePeerConnection();
    } else {
        MOSAIC_LOG_DEBUG("Received unknown message type: {}", type);
    }
}

void SignalingClient::SendWsMessage(const Json::Value& message) const {
    if (ws_client_->isConnected()) {
        ws_client_->sendJson(message);
    }
}

webrtc::IceCandidateInterface* ResolveIceCandidate(Json::Value const& ice_candidate) {
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
