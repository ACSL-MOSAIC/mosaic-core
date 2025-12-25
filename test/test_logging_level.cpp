//
// Created by Yeonhyuk Kim on 2025. 11. 17.
//

#include <chrono>
#include <memory>
#include <thread>

#include <gtest/gtest.h>

#include <rtc_sender/logger/log.h>
#include <rtc_sender/logger/i_logger.h>
#include <rtc_sender/signaling/websocket_client.h>

class LoggingLevelTester : public rtc_sender::log::ILogger {
public:
    ~LoggingLevelTester() override = default;

    void LOG(const std::string &message, const rtc_sender::log::LogLevel log_level) {
        if (log_level == rtc_sender::log::DEBUG) {
            DEBUG_LOG_COUNT++;
        } else if (log_level == rtc_sender::log::WARNING) {
            WARNING_LOG_COUNT++;
        } else if (log_level == rtc_sender::log::INFO) {
            INFO_LOG_COUNT++;
        } else if (log_level == rtc_sender::log::ERROR) {
            ERROR_LOG_COUNT++;
        }
    }

    int DEBUG_LOG_COUNT = 0;
    int WARNING_LOG_COUNT = 0;
    int INFO_LOG_COUNT = 0;
    int ERROR_LOG_COUNT = 0;
};

class LoggingLevelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 테스트 시작 전 초기화
        rtc_sender::log::RegisterLogger<LoggingLevelTester>();
        auto logger = rtc_sender::log::GetLogger();
        logger_tester_ = std::dynamic_pointer_cast<LoggingLevelTester>(logger);
    }

    void TearDown() override {
        // 테스트 완료 후 정리
    }

    std::shared_ptr<LoggingLevelTester> logger_tester_;
};

// 기본 로깅 테스트
TEST_F(LoggingLevelTest, SimpleLogging) {
    rtc_sender::log::SetLogLevel(rtc_sender::log::INFO);
    RTC_SENDER_LOG_INFO("Hello!");

    EXPECT_EQ(logger_tester_->INFO_LOG_COUNT, 1);
}

TEST_F(LoggingLevelTest, LevelInfoLogDebug) {
    rtc_sender::log::SetLogLevel(rtc_sender::log::INFO);
    RTC_SENDER_LOG_DEBUG("Hello!");

    EXPECT_EQ(logger_tester_->DEBUG_LOG_COUNT, 0);
}

TEST_F(LoggingLevelTest, LevelDebugLogInfo) {
    rtc_sender::log::SetLogLevel(rtc_sender::log::DEBUG);
    RTC_SENDER_LOG_INFO("Hello!");

    EXPECT_EQ(logger_tester_->INFO_LOG_COUNT, 1);
}

TEST_F(LoggingLevelTest, LevelDebugLogDebug) {
    rtc_sender::log::SetLogLevel(rtc_sender::log::DEBUG);
    RTC_SENDER_LOG_DEBUG("Hello!");

    EXPECT_EQ(logger_tester_->DEBUG_LOG_COUNT, 1);
}
