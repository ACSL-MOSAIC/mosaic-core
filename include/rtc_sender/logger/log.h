//
// Created by yhkim on 25. 7. 2.
//

#ifndef BA_GCS_RTC_SENDER_LOGGER_MACROS_H
#define BA_GCS_RTC_SENDER_LOGGER_MACROS_H

#include "i_logger.h"
#include "log_service.h"

namespace rtc_sender::log {
#define RTC_SENDER_LOG_DEBUG(...) rtc_sender::log::GetLogger()->LOG_DEBUG(__VA_ARGS__)

#define RTC_SENDER_LOG_WARNING(...) rtc_sender::log::GetLogger()->LOG_WARNING(__VA_ARGS__)

#define RTC_SENDER_LOG_INFO(...) rtc_sender::log::GetLogger()->LOG_INFO(__VA_ARGS__)

#define RTC_SENDER_LOG_ERROR(...) rtc_sender::log::GetLogger()->LOG_ERROR(__VA_ARGS__)
} // namespace rtc_sender::log

#endif  // BA_GCS_RTC_SENDER_LOGGER_MACROS_H
