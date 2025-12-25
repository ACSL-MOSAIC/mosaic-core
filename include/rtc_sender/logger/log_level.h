//
// Created by yhkim on 25. 7. 3.
//

#ifndef BA_GCS_RTC_SENDER_LOG_LEVEL_H
#define BA_GCS_RTC_SENDER_LOG_LEVEL_H

namespace rtc_sender::log {
    enum LogLevel {
        DEBUG = 0,
        WARNING = 1,
        INFO = 2,
        ERROR = 3,
    };
}

#endif  // BA_GCS_RTC_SENDER_LOG_LEVEL_H
