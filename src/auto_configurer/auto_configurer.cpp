//
// Created by yhkim on 12/28/25.
//

#include <mosaic/auto_configurer/auto_configurer.h>
#include <mosaic/auto_configurer/config_reader/config_reader_resolver.h>
#include <mosaic/auto_configurer/connector/a_dc_handler_configurer.h>
#include <mosaic/auto_configurer/connector/a_mt_handler_configurer.h>
#include <mosaic/auto_configurer/connector/a_parallel_dc_handler_configurer.h>
#include <mosaic/auto_configurer/connector/connector_resolver.h>
#include <mosaic/core/mosaic_connector_factory.h>

using namespace mosaic::auto_configurer;
using namespace mosaic::core;

void AutoConfigurer::AutoConfigure(const std::string& config_file_path) {
    ReadConfigs(config_file_path);
    CreateMosaicConnector();
    ResolveConnectors();
    BeforeConfigure();
    ConfigureConnectors();
    AfterConfigure();
}

void AutoConfigurer::ReadConfigs(const std::string& config_file_path) {
    config_reader_ = ConfigReaderResolver::GetInstance().GetConfigReader(config_file_path);

    auto configs = config_reader_->LoadConfigs(config_file_path);
    connector_configs_ = std::make_shared<ConnectorConfigs>(configs);
}

void AutoConfigurer::CreateMosaicConnector() {
    auto mosaic_connector_factory = MosaicConnectorFactory(std::make_shared<ServerConfig>(connector_configs_->server));

    mosaic_connector_ = mosaic_connector_factory.Create();
}

void AutoConfigurer::ResolveConnectors() {
    auto& resolver = ConnectorResolver::GetInstance();
    for (const auto& connector_config : connector_configs_->connectors) {
        if (!resolver.IsSupportedType(connector_config.type)) {
            MOSAIC_LOG_ERROR("Unsupported connector type: {}", connector_config.type);
            continue;
        }

        const auto configurable_connector = resolver.GetConfigurableConnector(connector_config.type);
        configurable_connector->SetConfig(connector_config);
        configurable_connectors_.push_back(configurable_connector);
    }
}

void AutoConfigurer::ConfigureConnectors() {
    for (const auto& configurable_connector : configurable_connectors_) {
        configurable_connector->Configure();

        if (const auto dc_ptr = dynamic_cast<ADCHandlerConfigurer*>(configurable_connector.get())) {
            const auto handler = dc_ptr->GetHandler();
            mosaic_connector_->AddDataChannelHandler(handler);

            dc_handler_map_[handler->GetLabel()] = handler;
        } else if (const auto dc_p_ptr = dynamic_cast<AParallelDCHandlerConfigurer*>(configurable_connector.get())) {
            const auto handlers = dc_p_ptr->GetHandlers();
            for (const auto& handler : handlers) {
                mosaic_connector_->AddDataChannelHandler(handler);

                dc_handler_map_[handler->GetLabel()] = handler;
            }
        } else if (const auto mt_ptr = dynamic_cast<AMTHandlerConfigurer*>(configurable_connector.get())) {
            const auto handler = mt_ptr->GetHandler();
            mosaic_connector_->AddMediaTrackHandler(handler);

            mt_handler_map_[handler->GetTrackName()] = handler;
        }
    }
}
