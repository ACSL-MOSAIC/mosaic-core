//
// Created by yhkim on 25. 7. 1.
//

#ifndef MOSAIC_RTC_CORE_DATA_CHANNEL_SENDABLE_H
#define MOSAIC_RTC_CORE_DATA_CHANNEL_SENDABLE_H

#include <mosaic/handlers/data_channel/a_data_channel_handler.h>
#include <mosaic/logger/log.h>

// Forward declaration for Json::Value
namespace Json {
class Value;
}

namespace mosaic::handlers {
class DataChannelSendable : public ADataChannelHandler {
  public:
    explicit DataChannelSendable(const std::string& label) : ADataChannelHandler(label) {}

    ~DataChannelSendable() override = default;

    bool Sendable() const;

    void Send(const webrtc::DataBuffer& buffer) const override;

    void SendString(const std::string& string) const;

    void SendStringAsByte(const std::string& string) const;

    void SendJson(const Json::Value& json_data) const;
};
}  // namespace mosaic::handlers

#endif  // MOSAIC_RTC_CORE_DATA_CHANNEL_SENDABLE_H
