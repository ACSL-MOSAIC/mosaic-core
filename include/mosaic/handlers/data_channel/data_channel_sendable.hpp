//
// Created by yhkim on 25. 7. 1.
//

#ifndef MOSAIC_CORE_DATA_CHANNEL_SENDABLE_HPP
#define MOSAIC_CORE_DATA_CHANNEL_SENDABLE_HPP

#include <json/json.h>

#include "data_channel_handler.hpp"

namespace mosaic::handlers {
class DataChannelSendable : public ADataChannelHandler {
  public:
    explicit DataChannelSendable(const std::string& label) : ADataChannelHandler(label) {}

    ~DataChannelSendable() override = default;

    bool Sendable() const;

    void Send(const webrtc::DataBuffer& buffer, bool isAsync = false) const;

    void SendString(const std::string& string, bool isAsync = false) const;

    void SendStringAsByte(const std::string& string, bool isAsync = false) const;

    void SendJson(const Json::Value& json_data, bool isAsync = false) const;
};
}  // namespace mosaic::handlers

#endif  // MOSAIC_CORE_DATA_CHANNEL_SENDABLE_HPP
