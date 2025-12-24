//
// Created by yhkim on 25. 7. 3.
//

#ifndef WEBRTC_LOG_H
#define WEBRTC_LOG_H

namespace rtc_sender::log {
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
} // namespace rtc_sender::log

#endif  // WEBRTC_LOG_H
