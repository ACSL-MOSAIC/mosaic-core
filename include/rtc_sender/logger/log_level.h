//
// Created by yhkim on 25. 7. 3.
//

#ifndef LOG_LEVEL_H
#define LOG_LEVEL_H

namespace rtc_sender::log {
    enum LogLevel {
        DEBUG = 0,
        WARNING = 1,
        INFO = 2,
        ERROR = 3,
    };
}

#endif  // LOG_LEVEL_H
