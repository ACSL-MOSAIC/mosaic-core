//
// Created by 김연혁 on 2025. 7. 16..
//

#include <gtest/gtest.h>
#include "rtc_sender/signaling/websocket_client.h"
#include <memory>
#include <chrono>
#include <thread>

using namespace rtc_sender::signaling;

class WebSocketClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 테스트 시작 전 초기화
        client = std::make_unique<WebSocketClient>();
    }

    void TearDown() override {
        // 테스트 완료 후 정리
        if (client) {
            client->disconnect();
        }
    }

    std::unique_ptr<WebSocketClient> client;
};

// 기본 연결 테스트
TEST_F(WebSocketClientTest, ConnectToValidServer) {
    // 로컬 테스트 서버나 공개 테스트 서버 사용
    const std::string test_server_url = "ws://ws.ifelse.io";

    bool connected = false;
    client->setOnConnected([&connected]() {
        connected = true;
    });

    client->connect(test_server_url);

    // 연결 완료까지 잠시 대기
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    EXPECT_TRUE(connected);
}

TEST_F(WebSocketClientTest, ConnectToValidSSLServer) {
    // WSS 테스트 서버 사용
    const std::string test_server_url = "wss://echo.websocket.org";

    bool connected = false;
    client->setOnConnected([&connected]() {
        connected = true;
    });

    client->connect(test_server_url);

    // SSL 핸드셰이크 포함하여 연결 완료까지 대기
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    EXPECT_TRUE(connected);
}

// 무효한 URL 연결 테스트
TEST_F(WebSocketClientTest, ConnectToInvalidServer) {
    const std::string invalid_url = "ws://invalid-server-that-does-not-exist.com";

    bool connection_failed = false;
    client->setOnError([&connection_failed](const std::string &) {
        connection_failed = true;
    });

    client->connect(invalid_url);

    // 에러 발생까지 잠시 대기
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    EXPECT_TRUE(connection_failed);
}

// 메시지 전송 테스트
TEST_F(WebSocketClientTest, SendMessage) {
    const std::string test_server_url = "wss://echo.websocket.org";
    const std::string test_message = "Hello, WebSocket!";

    bool connected = false;
    bool message_received = false;
    std::string received_message;

    client->setOnConnected([&connected]() {
        connected = true;
    });

    client->setOnMessage([&message_received, &received_message](const std::string &message) {
        message_received = true;
        received_message = message;
    });

    client->connect(test_server_url);

    // 연결 완료까지 대기
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ASSERT_TRUE(connected);

    // 메시지 전송
    client->send(test_message);

    // 응답 받을 때까지 대기
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    EXPECT_TRUE(message_received);
    EXPECT_EQ(received_message, test_message);
}

// 연결 해제 테스트
TEST_F(WebSocketClientTest, DisconnectFromServer) {
    const std::string test_server_url = "wss://echo.websocket.org";

    bool connected = false;
    bool disconnected = false;

    client->setOnConnected([&connected]() {
        connected = true;
    });

    client->setOnDisconnected([&disconnected]() {
        disconnected = true;
    });

    client->connect(test_server_url);

    // 연결 완료까지 대기
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ASSERT_TRUE(connected);

    // 연결 해제
    client->disconnect();

    // 해제 완료까지 대기
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    EXPECT_TRUE(disconnected);
}

// 멀티스레드 환경에서의 안전성 테스트
TEST_F(WebSocketClientTest, ThreadSafety) {
    const std::string test_server_url = "wss://echo.websocket.org";

    std::atomic<bool> connected{false};
    std::atomic<int> messages_sent{0};
    std::atomic<int> messages_received{0};

    client->setOnConnected([&connected]() {
        connected = true;
    });

    client->setOnMessage([&messages_received](const std::string &) {
        messages_received++;
    });

    client->connect(test_server_url);

    // 연결 완료까지 대기
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ASSERT_TRUE(connected);

    // 여러 스레드에서 동시에 메시지 전송
    std::vector<std::thread> threads;
    const int num_threads = 5;
    const int messages_per_thread = 10;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&, i]() {
            for (int j = 0; j < messages_per_thread; ++j) {
                std::string message = "Message from thread " + std::to_string(i) + ", msg " + std::to_string(j);
                client->send(message);
                messages_sent++;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }

    // 모든 스레드 완료 대기
    for (auto &thread: threads) {
        thread.join();
    }

    // 메시지 수신 완료까지 대기
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    EXPECT_EQ(messages_sent, num_threads * messages_per_thread);
    EXPECT_GE(messages_received, messages_sent); // 수신된 메시지가 전송된 메시지보다 많거나 같아야 함
}
