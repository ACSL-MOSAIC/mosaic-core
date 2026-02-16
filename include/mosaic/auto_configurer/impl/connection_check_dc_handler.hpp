//
// Created by yhkim on 2026. 2. 16.
//

#ifndef MOSAIC_CORE_CONNECTION_CHECK_DC_HANDLER_HPP
#define MOSAIC_CORE_CONNECTION_CHECK_DC_HANDLER_HPP

#include "../../handlers/data_channel/data_channel_receivable.hpp"
#include "../../handlers/data_channel/data_channel_sendable.hpp"
#include "../connector/configurable_connectors.hpp"

namespace mosaic::auto_configurer::impl {
class ConnectionCheckDCHandlerConfigurer : public AMultipleDCHandlerConfigurer {
  public:
    ConnectionCheckDCHandlerConfigurer() = default;

    std::string GetConnectorType() const override {
        return "connection-check";
    }
    void Configure() override;
};

class ConnectionCheckDataChannelSender : public handlers::DataChannelSendable {
  public:
    explicit ConnectionCheckDataChannelSender(const std::string& label) : DataChannelSendable(label) {}
};

class ConnectionCheckDataChannelReceiver : public handlers::DataChannelStringReceivable {
  public:
    explicit ConnectionCheckDataChannelReceiver(const std::string& label) : DataChannelStringReceivable(label) {}

    void SetSender(const std::shared_ptr<ConnectionCheckDataChannelSender>& sender) {
        sender_ = sender;
    }

    void HandleData(const std::string& data) override {
        sender_->SendString(data, true);
    }

  private:
    std::shared_ptr<ConnectionCheckDataChannelSender> sender_;
};

}  // namespace mosaic::auto_configurer::impl

#endif  // MOSAIC_CORE_CONNECTION_CHECK_DC_HANDLER_HPP
