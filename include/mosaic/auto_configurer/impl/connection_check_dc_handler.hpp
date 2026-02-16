//
// Created by yhkim on 2026. 2. 16.
//

#ifndef MOSAIC_CORE_CONNECTION_CHECK_DC_HANDLER_H
#define MOSAIC_CORE_CONNECTION_CHECK_DC_HANDLER_H

#include "mosaic/auto_configurer/connector/a_dc_handler_configurer.h"
#include "mosaic/handlers/data_channel/data_channel_receivable.h"
#include "mosaic/handlers/data_channel/data_channel_sendable.h"
#include "mosaic/handlers/media_track/a_media_track_handler.h"

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

#endif  // MOSAIC_CORE_CONNECTION_CHECK_DC_HANDLER_H
