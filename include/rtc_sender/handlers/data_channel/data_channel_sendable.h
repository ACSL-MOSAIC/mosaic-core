//
// Created by yhkim on 25. 7. 1.
//

#ifndef BA_GCS_RTC_SENDER_DATA_CHANNEL_SENDABLE_H
#define BA_GCS_RTC_SENDER_DATA_CHANNEL_SENDABLE_H

#include "a_data_channel_handler.h"
#include "rtc_sender/logger/log.h"
#include "rtc_sender/webrtc_forward_decl.h"

// Forward declaration for Json::Value
namespace Json {
    class Value;
}

namespace rtc_sender::handlers {
    class DataChannelSendable : public ADataChannelHandler {
    public:
        explicit DataChannelSendable(const std::string &label)
            : ADataChannelHandler(label) {
        }

        ~DataChannelSendable() override = default;

        bool Sendable() const;

        void Send(const webrtc::DataBuffer &buffer) const;

        void SendString(const std::string &string) const;

        void SendStringAsByte(const std::string &string) const;

        void SendJson(const Json::Value &json_data) const;
    };
} // namespace rtc_sender::handlers

#endif  // BA_GCS_RTC_SENDER_DATA_CHANNEL_SENDABLE_H
