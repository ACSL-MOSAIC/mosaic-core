//
// Created by yhkim on 25. 7. 3.
//

#ifndef MOSAIC_RTC_CORE_WEBRTC_LOG_H
#define MOSAIC_RTC_CORE_WEBRTC_LOG_H

namespace mosaic::core_log {
enum WebRTCLogLevel {
    LS_VERBOSE,
    LS_INFO,
    LS_WARNING,
    LS_ERROR,
    LS_NONE,
};

void SetWebRTCLogLevel(WebRTCLogLevel level);

void SetWebRTCLogTimestamps(bool enable);

void SetWebRTCLogThreads(bool enable);
}  // namespace mosaic::core_log

#endif  // MOSAIC_RTC_CORE_WEBRTC_LOG_H
