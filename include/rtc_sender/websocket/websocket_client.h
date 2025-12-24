//
// Created by 김연혁 on 2025. 7. 16..
//

#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <thread>

#define _TURN_OFF_PLATFORM_STRING  // DO NOT ERASE THIS LINE AND CHANGE THE POSITION
#include <cpprest/json.h>
#include <cpprest/ws_client.h>

using namespace web;
using namespace web::websockets::client;

class WebSocketClient {
public:
  // 콜백 함수 타입 정의
  using OnConnectedCallback = std::function<void()>;
  using OnDisconnectedCallback = std::function<void()>;
  using OnMessageCallback = std::function<void(const std::string &)>;
  using OnErrorCallback = std::function<void(const std::string &)>;

  WebSocketClient();

  ~WebSocketClient();

  // 콜백 설정
  void setOnConnected(OnConnectedCallback callback);

  void setOnDisconnected(OnDisconnectedCallback callback);

  void setOnMessage(OnMessageCallback callback);

  void setOnError(OnErrorCallback callback);

  // 연결 관련
  void connect(const std::string &uri);

  void disconnect();

  // 메시지 전송
  void send(const std::string &message);

  void sendJson(const json::value &jsonMessage);

  // 상태 확인
  bool isConnected() const;

private:
  std::shared_ptr<websocket_callback_client> m_client = nullptr;
  std::atomic<bool> m_connected;
  std::string m_uri;

  // 콜백 함수들
  OnConnectedCallback m_onConnected;
  OnDisconnectedCallback m_onDisconnected;
  OnMessageCallback m_onMessage;
  OnErrorCallback m_onError;

  void connectInternal();

  // 내부 이벤트 처리
  void handleMessage(const websocket_incoming_message &message);

  void handleClose(websocket_close_status close_status, const std::string &reason, const std::error_code &);

  void setupEventHandlers();
};

#endif  // WEBSOCKET_CLIENT_H
