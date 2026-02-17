//
// Created by yhkim on 25. 7. 16.
//

#ifndef MOSAIC_CORE_WEBSOCKET_CLIENT_HPP
#define MOSAIC_CORE_WEBSOCKET_CLIENT_HPP

#include <functional>
#include <string>

#include <json/json.h>

#include "../utils.hpp"

namespace mosaic::core_signaling {
class WebSocketClientImpl;

class WebSocketClient : CheshireCat<WebSocketClientImpl> {
  public:
    // declare callback functions
    using OnConnectedCallback = std::function<void()>;
    using OnDisconnectedCallback = std::function<void()>;
    using OnMessageCallback = std::function<void(const std::string&)>;
    using OnErrorCallback = std::function<void(const std::string&)>;

    explicit WebSocketClient(impl_ptr<WebSocketClientImpl> impl);

    ~WebSocketClient() override;

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
    using CheshireCat::impl;
};
}  // namespace mosaic::core_signaling

#endif  // MOSAIC_CORE_WEBSOCKET_CLIENT_HPP
