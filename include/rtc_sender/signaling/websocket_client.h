//
// Created by yhkim on 25. 7. 16.
//

#ifndef BA_GCS_RTC_SENDER_WEBSOCKET_CLIENT_H
#define BA_GCS_RTC_SENDER_WEBSOCKET_CLIENT_H

#include <atomic>
#include <functional>
#include <memory>
#include <string>

#define TURN_OFF_PLATFORM_STRING  // DO NOT ERASE THIS LINE AND CHANGE THE POSITION
#include <cpprest/json.h>
#include <cpprest/ws_client.h>

using namespace web;
using namespace web::websockets::client;

namespace rtc_sender::signaling {
    class WebSocketClient {
    public:
        // declare callback functions
        using OnConnectedCallback = std::function<void()>;
        using OnDisconnectedCallback = std::function<void()>;
        using OnMessageCallback = std::function<void(const std::string &)>;
        using OnErrorCallback = std::function<void(const std::string &)>;

        WebSocketClient();

        ~WebSocketClient();

        void setOnConnected(OnConnectedCallback callback);

        void setOnDisconnected(OnDisconnectedCallback callback);

        void setOnMessage(OnMessageCallback callback);

        void setOnError(OnErrorCallback callback);

        void connect(const std::string &uri);

        void disconnect();

        void send(const std::string &message) const;

        void sendJson(const json::value &jsonMessage) const;

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

        void handleMessage(const websocket_incoming_message &message) const;

        void handleClose(websocket_close_status close_status, const std::string &reason, const std::error_code &);

        void setupEventHandlers();
    };
} // namespace rtc_sender::signaling

#endif  // BA_GCS_RTC_SENDER_WEBSOCKET_CLIENT_H
