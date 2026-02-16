//
// Created by yhkim on 2/17/26.
//

#include "mosaic/auto_configurer/impl/connection_check_dc_handler.hpp"

using namespace mosaic::auto_configurer;

void impl::ConnectionCheckDCHandlerConfigurer::Configure() {
    const auto sender = std::make_shared<ConnectionCheckDataChannelSender>(connector_config_->label + "-sender");
    const auto receiver = std::make_shared<ConnectionCheckDataChannelReceiver>(connector_config_->label + "-receiver");
    receiver->SetSender(sender);

    handlers_.push_back(receiver);
    handlers_.push_back(sender);
}
