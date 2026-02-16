//
// Created by yhkim on 2026. 2. 16.
//

#ifndef MOSAIC_CORE_CONNECTION_CHECK_DC_HANDLER_HPP
#define MOSAIC_CORE_CONNECTION_CHECK_DC_HANDLER_HPP

#include "../../handlers/data_channel/data_channel_receivable.hpp"
#include "../../handlers/data_channel/data_channel_sendable.hpp"
#include "../connector/configurable_connectors.hpp"

namespace mosaic::auto_configurer::impl {
class ConnectionCheckDCHandlerConfigurer : public ADCHandlerConfigurer {
  public:
    ConnectionCheckDCHandlerConfigurer() = default;

    std::string GetConnectorType() const override {
        return "connection-check";
    }
    void Configure() override;
};

class ConnectionCheckDataChannel : public handlers::DataChannelSendable, public handlers::DataChannelStringReceivable {
  public:
    explicit ConnectionCheckDataChannel(const std::string& label)
        : DataChannelSendable(label), DataChannelStringReceivable(label) {}

    void HandleData(const std::string& data) override {
        SendString(data, true);
    }
};

}  // namespace mosaic::auto_configurer::impl

#endif  // MOSAIC_CORE_CONNECTION_CHECK_DC_HANDLER_HPP
