//
// Created by yhkim on 25. 7. 1.
//

#ifndef MOSAIC_RTC_CORE_DATA_CHANNEL_RECEIVABLE_H
#define MOSAIC_RTC_CORE_DATA_CHANNEL_RECEIVABLE_H

#include <json/json.h>

#include "mosaic_rtc_core/logger/log.h"

#include "i_data_channel_receivable.h"

namespace mosaic::handlers {
template <typename ReceiveT>
class DataChannelReceivable : public IDataChannelReceivable {
  public:
    explicit DataChannelReceivable(const std::string& label) : IDataChannelReceivable(label) {}

    ~DataChannelReceivable() override = default;

    void OnMessage(const webrtc::DataBuffer& buffer) override {
        try {
            const auto json_data = ConvertDataBufferToJson(buffer);
            auto data = ConvertJsonToData(json_data);
            HandleData(data);
        } catch (const std::exception& e) {
            MOSAIC_LOG_ERROR("Error processing message: {}", e.what());
        }
    }

    virtual ReceiveT ConvertJsonToData(const Json::Value& json_data) = 0;

    virtual void HandleData(const ReceiveT&) {}
};
}  // namespace mosaic::handlers

#endif  // MOSAIC_RTC_CORE_DATA_CHANNEL_RECEIVABLE_H
