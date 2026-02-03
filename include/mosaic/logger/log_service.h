//
// Created by yhkim on 25. 7. 2.
//

#ifndef MOSAIC_RTC_CORE_LOG_SERVICE_H
#define MOSAIC_RTC_CORE_LOG_SERVICE_H

#include <memory>
#include <type_traits>

#include "log_level.h"

namespace mosaic::core_log {
class ILogger;

class LogService {
  public:
    static LogService& GetInstance() {
        static LogService instance;
        return instance;
    }

    LogService(const LogService&) = delete;

    LogService& operator=(const LogService&) = delete;

    LogLevel GetLogLevel() const {
        return log_level_;
    }

    LogLevel log_level_ = INFO;
    std::shared_ptr<ILogger> logger_ = DefaultLogger();

  private:
    LogService() = default;

    ~LogService() = default;

    static std::shared_ptr<ILogger> DefaultLogger();
};

template <typename Impl, typename... Args>
void RegisterLogger(Args&&... args) {
    static_assert(std::is_base_of_v<ILogger, Impl>, "Impl must inherit from ILogger");
    LogService::GetInstance().logger_ = std::make_unique<Impl>(args...);
}

inline std::shared_ptr<ILogger> GetLogger() {
    return LogService::GetInstance().logger_;
}

inline void SetLogLevel(const LogLevel level) {
    LogService::GetInstance().log_level_ = level;
}

inline LogLevel GetLogLevel() {
    return LogService::GetInstance().log_level_;
}
}  // namespace mosaic::core_log

#endif  // MOSAIC_RTC_CORE_LOG_SERVICE_H
