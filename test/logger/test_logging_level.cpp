//
// Created by Yeonhyuk Kim on 2025. 11. 17.
//

#include <chrono>
#include <memory>
#include <thread>

#include <gtest/gtest.h>
#include <mosaic_rtc_core/logger/i_logger.h>
#include <mosaic_rtc_core/logger/log.h>
#include <mosaic_rtc_core/signaling/websocket_client.h>

class LoggingLevelTester : public mosaic::core_log::ILogger {
  public:
    ~LoggingLevelTester() override = default;

    void LOG(const std::string& message, const mosaic::core_log::LogLevel log_level) {
        if (log_level == mosaic::core_log::DEBUG) {
            DEBUG_LOG_COUNT++;
        } else if (log_level == mosaic::core_log::WARNING) {
            WARNING_LOG_COUNT++;
        } else if (log_level == mosaic::core_log::INFO) {
            INFO_LOG_COUNT++;
        } else if (log_level == mosaic::core_log::ERROR) {
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
        mosaic::core_log::RegisterLogger<LoggingLevelTester>();
        auto logger = mosaic::core_log::GetLogger();
        logger_tester_ = std::dynamic_pointer_cast<LoggingLevelTester>(logger);
    }

    void TearDown() override {
        // 테스트 완료 후 정리
    }

    std::shared_ptr<LoggingLevelTester> logger_tester_;
};

// 기본 로깅 테스트
TEST_F(LoggingLevelTest, SimpleLogging) {
    mosaic::core_log::SetLogLevel(mosaic::core_log::INFO);
    MOSAIC_LOG_INFO("Hello!");

    EXPECT_EQ(logger_tester_->INFO_LOG_COUNT, 1);
}

TEST_F(LoggingLevelTest, LevelInfoLogDebug) {
    mosaic::core_log::SetLogLevel(mosaic::core_log::INFO);
    MOSAIC_LOG_DEBUG("Hello!");

    EXPECT_EQ(logger_tester_->DEBUG_LOG_COUNT, 0);
}

TEST_F(LoggingLevelTest, LevelDebugLogInfo) {
    mosaic::core_log::SetLogLevel(mosaic::core_log::DEBUG);
    MOSAIC_LOG_INFO("Hello!");

    EXPECT_EQ(logger_tester_->INFO_LOG_COUNT, 1);
}

TEST_F(LoggingLevelTest, LevelDebugLogDebug) {
    mosaic::core_log::SetLogLevel(mosaic::core_log::DEBUG);
    MOSAIC_LOG_DEBUG("Hello!");

    EXPECT_EQ(logger_tester_->DEBUG_LOG_COUNT, 1);
}
