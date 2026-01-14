//
// Created by yhkim on 25. 7. 16.
//

#include <thread>
#include <utility>

#include <mosaic/logger/log.h>
#include <mosaic/signaling/websocket_client.h>

using namespace mosaic::core_signaling;

WebSocketClient::WebSocketClient() : m_connected(false) {}

WebSocketClient::~WebSocketClient() {
    if (m_connected) {
        disconnect();
    }
}

void WebSocketClient::setOnConnected(OnConnectedCallback callback) {
    m_onConnected = std::move(callback);
}

void WebSocketClient::setOnDisconnected(OnDisconnectedCallback callback) {
    m_onDisconnected = std::move(callback);
}

void WebSocketClient::setOnMessage(OnMessageCallback callback) {
    m_onMessage = std::move(callback);
}

void WebSocketClient::setOnError(OnErrorCallback callback) {
    m_onError = std::move(callback);
}

void WebSocketClient::connect(const std::string& uri) {
    m_uri = uri;
    m_connected = false;

    connectInternal();
}

void WebSocketClient::connectInternal() {
    // WebSocket 클라이언트 설정
    websocket_client_config config;

    // WSS 연결을 위한 SSL 설정
    if (m_uri.find("wss://") == 0) {
        config.set_validate_certificates(false);  // 개발용 - 실제 환경에서는 true로 설정
    }

    // 새로운 클라이언트 생성
    m_client = std::make_shared<websocket_callback_client>(config);

    // 이벤트 핸들러 설정
    setupEventHandlers();

    try {
        // 연결 시도
        const auto res = m_client->connect(m_uri)
                             .then([this] {
                                 m_connected = true;
                                 MOSAIC_LOG_INFO("WebSocket connected to {}", m_uri);

                                 if (m_onConnected) {
                                     m_onConnected();
                                 }
                             })
                             .wait();
        if (res != pplx::task_status::completed) {
            throw std::runtime_error("Failed to connect to WebSocket server");
        }
    } catch (const std::exception& e) {
        MOSAIC_LOG_ERROR("Connection failed: {}", e.what());
    }
}

void WebSocketClient::disconnect() {
    if (!m_connected) {
        return;
    }

    m_connected = false;

    const auto res = m_client->close()
                         .then([this] {
                             MOSAIC_LOG_INFO("WebSocket disconnected");

                             if (m_onDisconnected) {
                                 m_onDisconnected();
                             }
                         })
                         .wait();
    if (res != pplx::task_status::completed) {
        MOSAIC_LOG_ERROR("Failed to disconnect WebSocket client");
    }
}

void WebSocketClient::send(const std::string& message) const {
    if (!m_connected) {
        MOSAIC_LOG_ERROR("Failed to send message; not connected to WebSocket server.");
        return;
    }

    websocket_outgoing_message msg;
    msg.set_utf8_message(message);

    if (const auto res = m_client->send(msg).wait(); res != pplx::task_status::completed) {
        MOSAIC_LOG_ERROR("Failed to send message: {}", message);
        throw std::runtime_error("Failed to send message");
    }
}

void WebSocketClient::sendJson(const json::value& jsonMessage) const {
    send(jsonMessage.serialize());
}

bool WebSocketClient::isConnected() const {
    return m_connected;
}

void WebSocketClient::setupEventHandlers() {
    // 메시지 수신 핸들러
    m_client->set_message_handler([this](const websocket_incoming_message& message) { handleMessage(message); });

    // 연결 종료 핸들러
    m_client->set_close_handler([this](const websocket_close_status close_status,
                                       const std::string& reason,
                                       const std::error_code& error) { handleClose(close_status, reason, error); });
}

void WebSocketClient::handleMessage(const websocket_incoming_message& message) const {
    try {
        if (message.message_type() == websocket_message_type::ping) {
            MOSAIC_LOG_DEBUG("Received ping message");
            auto pong_message = websocket_outgoing_message();
            pong_message.set_pong_message("pong");
            m_client->send(pong_message).wait();
            return;  // Ping 메시지는 Pong으로 응답하고 무시
        }
        if (message.message_type() == websocket_message_type::pong) {
            MOSAIC_LOG_DEBUG("Received pong message");
            return;  // Pong 메시지는 무시
        }
        if (message.message_type() == websocket_message_type::close) {
            m_client->close().wait();
            MOSAIC_LOG_DEBUG("Received close message from server");
            return;
        }
        if (message.message_type() == websocket_message_type::text_message) {
            auto ts = message.extract_string()
                          .then([this](const std::string& body) {
                              if (m_onMessage) {
                                  m_onMessage(body);
                              }
                          })
                          .wait();
            if (ts != pplx::task_status::completed) {
                m_onError("Task did not complete successfully while handling message.");
            }
        }
    } catch (const std::exception& e) {
        const std::string error = "Message handling error: " + std::string(e.what());
        if (m_onError) {
            m_onError(error);
        }
    }
}

void WebSocketClient::handleClose(websocket_close_status close_status,
                                  const std::string& reason,
                                  const std::error_code&) {
    m_connected = false;

    /*
    *enum class websocket_close_status
    {
        normal = 1000,
        going_away = 1001,
        protocol_error = 1002,
        unsupported = 1003, // or data_mismatch
        abnormal_close = 1006,
        inconsistent_datatype = 1007,
        policy_violation = 1008,
        too_large = 1009,
        negotiate_error = 1010,
        server_terminate = 1011,
    };
     */
    MOSAIC_LOG_INFO("WebSocket closed. Status: {} Reason: {} Error Code: {}",
                    static_cast<int>(close_status),
                    reason,
                    std::error_code().message());

    if (m_onDisconnected) {
        m_onDisconnected();
    }

    // Reconnection logic start
    std::thread reconnect_thread([this] {
        MOSAIC_LOG_ERROR("Reconnecting to WebSocket server in 5 seconds...");

        std::this_thread::sleep_for(std::chrono::seconds(5));  // Wait before reconnecting
        this->connectInternal();
    });
    reconnect_thread.detach();
}
