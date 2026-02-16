//
// Created by yhkim on 25. 7. 1.
//

#ifndef MOSAIC_CORE_DATA_CHANNEL_RECEIVABLE_HPP
#define MOSAIC_CORE_DATA_CHANNEL_RECEIVABLE_HPP

#include <vector>

#include <json/json.h>

#include "../../logger/log.hpp"
#include "data_channel_handler.hpp"

namespace mosaic::handlers {
class IDataChannelReceivable : public ADataChannelHandler {
  public:
    explicit IDataChannelReceivable(const std::string& label) : ADataChannelHandler(label) {}

    ~IDataChannelReceivable() override = default;

    void OnMessage(const webrtc::DataBuffer& buffer) override = 0;
};

template <typename ReceiveT>
class DataChannelReceivable : public IDataChannelReceivable {
  public:
    explicit DataChannelReceivable(const std::string& label) : IDataChannelReceivable(label) {}

    ~DataChannelReceivable() override = default;

    void OnMessage(const webrtc::DataBuffer& buffer) override {
        try {
            auto data = ConvertBufferToData(buffer);
            HandleData(data);
        } catch (const std::exception& e) {
            MOSAIC_LOG_ERROR("Error processing message: {}", e.what());
        }
    }

  protected:
    virtual ReceiveT ConvertBufferToData(const webrtc::DataBuffer& buffer) = 0;

    virtual void HandleData(const ReceiveT&) {}
};

class DataChannelStringReceivable : public DataChannelReceivable<std::string> {
  public:
    explicit DataChannelStringReceivable(const std::string& label) : DataChannelReceivable(label) {}

    ~DataChannelStringReceivable() override = default;

  protected:
    std::string ConvertBufferToData(const webrtc::DataBuffer& buffer) override;

    virtual void HandleData(const std::string&) override {}
};

class DataChannelJsonReceivable : public DataChannelReceivable<Json::Value> {
  public:
    explicit DataChannelJsonReceivable(const std::string& label) : DataChannelReceivable(label) {}

    ~DataChannelJsonReceivable() override = default;

  protected:
    Json::Value ConvertBufferToData(const webrtc::DataBuffer& buffer) override;

    virtual void HandleData(const Json::Value&) override {}
};

class DataChannelByteReceivable : public DataChannelReceivable<std::vector<uint8_t>> {
  public:
    explicit DataChannelByteReceivable(const std::string& label) : DataChannelReceivable(label) {}

    ~DataChannelByteReceivable() override = default;

  protected:
    std::vector<uint8_t> ConvertBufferToData(const webrtc::DataBuffer& buffer) override;

    virtual void HandleData(const std::vector<uint8_t>&) override {}
};
}  // namespace mosaic::handlers

#endif  // MOSAIC_CORE_DATA_CHANNEL_RECEIVABLE_HPP
