//
// Created by yhkim on 25. 7. 2.
//

#ifndef MOSAIC_CORE_I_LOGGER_HPP
#define MOSAIC_CORE_I_LOGGER_HPP

#include <string>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

#include "log_level.hpp"
#include "log_service.hpp"

namespace mosaic::core_log {
class ILogger {
  public:
    virtual ~ILogger() = default;

    virtual void LOG(const std::string& message, LogLevel log_level) = 0;

    void LOG_DEBUG(const std::string& message) {
        if (IsDebugEnabled()) {
            LOG(message, DEBUG);
        }
    }

    template <typename... Args>
    void LOG_DEBUG(const std::string& format, Args&&... args) {
        if (IsDebugEnabled()) {
            // Assuming a simple string formatting function is available
            const std::string formatted_message = fmt::format(format, std::forward<Args>(args)...);
            LOG(formatted_message, DEBUG);
        }
    }

    void LOG_WARNING(const std::string& message) {
        if (IsWarningEnabled()) {
            LOG(message, WARNING);
        }
    }

    template <typename... Args>
    void LOG_WARNING(const std::string& format, Args&&... args) {
        if (IsWarningEnabled()) {
            // Assuming a simple string formatting function is available
            const std::string formatted_message = fmt::format(format, std::forward<Args>(args)...);
            LOG(formatted_message, WARNING);
        }
    }

    void LOG_INFO(const std::string& message) {
        if (IsInfoEnabled()) {
            LOG(message, INFO);
        }
    }

    template <typename... Args>
    void LOG_INFO(const std::string& format, Args&&... args) {
        if (IsInfoEnabled()) {
            // Assuming a simple string formatting function is available
            const std::string formatted_message = fmt::format(format, std::forward<Args>(args)...);
            LOG(formatted_message, INFO);
        }
    }

    void LOG_ERROR(const std::string& message) {
        if (IsErrorEnabled()) {
            LOG(message, ERROR);
        }
    }

    template <typename... Args>
    void LOG_ERROR(const std::string& format, Args&&... args) {
        if (IsErrorEnabled()) {
            // Assuming a simple string formatting function is available
            const std::string formatted_message = fmt::format(format, std::forward<Args>(args)...);
            LOG(formatted_message, ERROR);
        }
    }

    static LogLevel GetLogLevel() {
        return LogService::GetInstance().GetLogLevel();
    }

    static bool IsDebugEnabled() {
        return LogService::GetInstance().GetLogLevel() <= DEBUG;
    }

    static bool IsInfoEnabled() {
        return LogService::GetInstance().GetLogLevel() <= INFO;
    }

    static bool IsWarningEnabled() {
        return LogService::GetInstance().GetLogLevel() <= WARNING;
    }

    static bool IsErrorEnabled() {
        return LogService::GetInstance().GetLogLevel() <= ERROR;
    }
};
}  // namespace mosaic::core_log

#endif  // MOSAIC_CORE_I_LOGGER_HPP
