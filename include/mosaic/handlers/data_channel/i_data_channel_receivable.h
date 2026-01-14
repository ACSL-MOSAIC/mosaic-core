//
// Created by yhkim on 25. 7. 2.
//

#ifndef MOSAIC_RTC_CORE_I_DATA_CHANNEL_RECEIVABLE_H
#define MOSAIC_RTC_CORE_I_DATA_CHANNEL_RECEIVABLE_H

#include <mosaic/handlers/data_channel/a_data_channel_handler.h>

namespace Json {
class Value;
}

namespace mosaic::handlers {
class IDataChannelReceivable : public ADataChannelHandler {
  public:
    explicit IDataChannelReceivable(const std::string& label) : ADataChannelHandler(label) {}

    ~IDataChannelReceivable() override = default;

    static Json::Value ConvertDataBufferToJson(const webrtc::DataBuffer& buffer);

    void OnMessage(const webrtc::DataBuffer& buffer) override = 0;
};
}  // namespace mosaic::handlers

#endif  // MOSAIC_RTC_CORE_I_DATA_CHANNEL_RECEIVABLE_H
