//
// Created by yhkim on 25. 7. 2.
//

#ifndef MOSAIC_CORE_CONSOLE_LOGGER_HPP
#define MOSAIC_CORE_CONSOLE_LOGGER_HPP

#include <chrono>
#include <iomanip>
#include <iostream>

#include "i_logger.hpp"

namespace mosaic::core_log {
class ConsoleLogger : public ILogger {
  public:
    ~ConsoleLogger() override = default;

    void LOG(const std::string& message, const LogLevel log_level) override {
        const auto now = std::chrono::system_clock::now();
        const auto time_t = std::chrono::system_clock::to_time_t(now);
        const auto tm = *std::localtime(&time_t);
        std::cout << "[" << log_level << "]" << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S %Z") << "] " << message
                  << std::endl;
    }
};
}  // namespace mosaic::core_log

#endif  // MOSAIC_CORE_CONSOLE_LOGGER_HPP
