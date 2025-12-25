//
// Created by yhkim on 25. 7. 1.
//

#ifndef BA_GCS_RTC_SENDER_DATA_CHANNEL_RECEIVABLE_H
#define BA_GCS_RTC_SENDER_DATA_CHANNEL_RECEIVABLE_H

#include "i_data_channel_receivable.h"
#include "rtc_sender/logger/log.h"
#include <json/json.h>

namespace rtc_sender::handlers {
    template<typename ReceiveT>
    class DataChannelReceivable : public IDataChannelReceivable {
    public:
        DataChannelReceivable(const std::string &label)
            : IDataChannelReceivable(label) {
        }

        ~DataChannelReceivable() override = default;

        void OnMessage(const webrtc::DataBuffer &buffer) override {
            try {
                auto json_data = ConvertDataBufferToJson(buffer);
                auto data = ConvertJsonToData(json_data);
                HandleData(data);
            } catch (const std::exception &e) {
                RTC_SENDER_LOG_ERROR("Error processing message: {}", e.what());
            }
        }

        virtual ReceiveT ConvertJsonToData(const Json::Value &json_data) = 0;

        virtual void HandleData(const ReceiveT &) {
        }
    };
} // namespace rtc_sender::handlers

#endif  // BA_GCS_RTC_SENDER_DATA_CHANNEL_RECEIVABLE_H
