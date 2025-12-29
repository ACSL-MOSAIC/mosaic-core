//
// Created by yhkim on 25. 7. 3.
//

#include <mosaic_rtc_core/logger/webrtc_log.h>
#include <rtc_base/logging.h>

void rtc_sender::log::SetWebRTCLogLevel(const WebRTCLogLevel level) {
    switch (level) {
        case LS_INFO:
            rtc::LogMessage::LogToDebug(rtc::LS_INFO);
            break;
        case LS_WARNING:
            rtc::LogMessage::LogToDebug(rtc::LS_WARNING);
            break;
        case LS_ERROR:
            rtc::LogMessage::LogToDebug(rtc::LS_ERROR);
            break;
        case LS_NONE:
            rtc::LogMessage::LogToDebug(rtc::LS_NONE);
            break;
        case LS_VERBOSE:
            rtc::LogMessage::LogToDebug(rtc::LS_VERBOSE);
            break;
    }
}

void rtc_sender::log::SetWebRTCLogTimestamps(const bool enable = true) {
    rtc::LogMessage::LogTimestamps(enable);
}

void rtc_sender::log::SetWebRTCLogThreads(const bool enable = true) {
    rtc::LogMessage::LogThreads(enable);
}
