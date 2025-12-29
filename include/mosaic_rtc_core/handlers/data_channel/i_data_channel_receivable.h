//
// Created by yhkim on 25. 7. 2.
//

#ifndef BA_GCS_RTC_SENDER_I_DATA_CHANNEL_RECEIVABLE_H
#define BA_GCS_RTC_SENDER_I_DATA_CHANNEL_RECEIVABLE_H

#include "a_data_channel_handler.h"
#include "mosaic_rtc_core/webrtc_forward_decl.h"

namespace Json {
    class Value;
}

namespace rtc_sender::handlers {
    class IDataChannelReceivable : public ADataChannelHandler {
    public:
        explicit IDataChannelReceivable(const std::string &label)
            : ADataChannelHandler(label) {
        }

        ~IDataChannelReceivable() override = default;

        static Json::Value ConvertDataBufferToJson(const webrtc::DataBuffer &buffer);

        void OnMessage(const webrtc::DataBuffer &buffer) override = 0;
    };
} // namespace rtc_sender::handlers

#endif  // BA_GCS_RTC_SENDER_I_DATA_CHANNEL_RECEIVABLE_H
