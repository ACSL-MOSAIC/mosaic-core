//
// Created by yhkim on 25. 7. 16.
//

#include "mosaic/signaling/websocket_client.hpp"

#include "websocket_client_impl.hpp"

using namespace mosaic::core_signaling;

WebSocketClient::WebSocketClient(impl_ptr<WebSocketClientImpl> impl) : CheshireCat(std::move(impl)) {}

WebSocketClient::~WebSocketClient() {
    if (impl()->isConnected()) {
        impl()->disconnect();
    }
}

void WebSocketClient::setOnConnected(OnConnectedCallback callback) {
    impl()->setOnConnected(callback);
}

void WebSocketClient::setOnDisconnected(OnDisconnectedCallback callback) {
    impl()->setOnDisconnected(callback);
}

void WebSocketClient::setOnMessage(OnMessageCallback callback) {
    impl()->setOnMessage(callback);
}

void WebSocketClient::setOnError(OnErrorCallback callback) {
    impl()->setOnError(callback);
}

void WebSocketClient::connect(const std::string& uri) {
    impl()->connect(uri);
}

void WebSocketClient::disconnect() {
    impl()->disconnect();
}

void WebSocketClient::send(const std::string& message) const {
    impl()->send(message);
}

void WebSocketClient::sendJson(const Json::Value& jsonMessage) const {
    impl()->sendJson(jsonMessage);
}

bool WebSocketClient::isConnected() const {
    return impl()->isConnected();
}
