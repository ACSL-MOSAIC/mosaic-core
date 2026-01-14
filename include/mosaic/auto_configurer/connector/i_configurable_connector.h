//
// Created by yhkim on 12/28/25.
//

#ifndef MOSAIC_AUTO_CONFIGURER_I_CONFIGURABLE_CONNECTOR_H
#define MOSAIC_AUTO_CONFIGURER_I_CONFIGURABLE_CONNECTOR_H

#include <mosaic_rtc_core/configs_decl.h>
#include <mosaic_rtc_core/core/mosaic_connector.h>

namespace mosaic::auto_configurer {
class IConfigurableConnector {
  public:
    virtual ~IConfigurableConnector() = default;

    virtual std::string GetConnectorType() const = 0;

    void SetConfig(const core::ConnectorConfig& connector_config) {
        connector_config_ = connector_config;
    }

    core::ConnectorConfig GetConfig() const {
        return connector_config_;
    }

    virtual void Configure(std::shared_ptr<core::MosaicConnector> mosaic_container) = 0;

  protected:
    core::ConnectorConfig connector_config_;
};
}  // namespace mosaic::auto_configurer

#endif  // MOSAIC_AUTO_CONFIGURER_I_CONFIGURABLE_CONNECTOR_H
