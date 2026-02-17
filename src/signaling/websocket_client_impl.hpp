//
// Created by yhkim on 2026. 2. 17.
//

#ifndef MOSAIC_CORE_WEBSOCKET_CLIENT_IMPL_HPP
#define MOSAIC_CORE_WEBSOCKET_CLIENT_IMPL_HPP

#define _TURN_OFF_PLATFORM_STRING  // DO NOT ERASE THIS LINE AND CHANGE THE POSITION

#include <atomic>
#include <functional>
#include <memory>
#include <string>

#include <json/json.h>

// TODO: There is an issue where websocket is excluded when
//  installing cpprest via Homebrew on macOS.
#ifdef __APPLE__
#ifdef CPPREST_EXCLUDE_WEBSOCKETS
#undef CPPREST_EXCLUDE_WEBSOCKETS
#endif
#endif

#include <cpprest/ws_client.h>

using namespace web::websockets::client;

namespace mosaic::core_signaling {

class WebSocketClientImpl {
  public:
    // declare callback functions
    using OnConnectedCallback = std::function<void()>;
    using OnDisconnectedCallback = std::function<void()>;
    using OnMessageCallback = std::function<void(const std::string&)>;
    using OnErrorCallback = std::function<void(const std::string&)>;

    WebSocketClientImpl();

    ~WebSocketClientImpl();

    void setOnConnected(OnConnectedCallback callback);

    void setOnDisconnected(OnDisconnectedCallback callback);

    void setOnMessage(OnMessageCallback callback);

    void setOnError(OnErrorCallback callback);

    void connect(const std::string& uri);

    void disconnect();

    void send(const std::string& message) const;

    void sendJson(const Json::Value& jsonMessage) const;

    [[nodiscard]] bool isConnected() const;

  private:
    std::shared_ptr<websocket_callback_client> m_client = nullptr;
    std::atomic<bool> m_connected;
    std::string m_uri;

    OnConnectedCallback m_onConnected;
    OnDisconnectedCallback m_onDisconnected;
    OnMessageCallback m_onMessage;
    OnErrorCallback m_onError;

    void connectInternal();

    void handleMessage(const websocket_incoming_message& message) const;

    void handleClose(websocket_close_status close_status, const std::string& reason, const std::error_code&);

    void setupEventHandlers();
};
}  // namespace mosaic::core_signaling

#endif  // MOSAIC_CORE_WEBSOCKET_CLIENT_IMPL_HPP
