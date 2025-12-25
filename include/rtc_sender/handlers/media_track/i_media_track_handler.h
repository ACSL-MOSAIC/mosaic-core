//
// Created by yhkim on 25. 6. 22.
//

#ifndef BA_GCS_RTC_SENDER_I_MEDIA_TRACK_HANDLER_H
#define BA_GCS_RTC_SENDER_I_MEDIA_TRACK_HANDLER_H

#include <string>
#include "rtc_sender/webrtc_forward_decl.h"

namespace rtc_sender::handlers {
    class IMediaTrackHandler {
    public:
        explicit IMediaTrackHandler(const std::string &track_name) : track_name_(track_name) {
        }

        virtual ~IMediaTrackHandler() = default;

        virtual webrtc::scoped_refptr<webrtc::VideoTrackSourceInterface> CreateVideoTrackSource() = 0;

        virtual void Close() = 0;

        std::string GetTrackName() const {
            return track_name_;
        }

    protected:
        std::string track_name_;
    };
} // namespace rtc_sender::handlers

#endif  // BA_GCS_RTC_SENDER_I_MEDIA_TRACK_HANDLER_H
