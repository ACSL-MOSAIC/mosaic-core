//
// Created by yhkim on 12/31/25.
//

#ifndef MOSAIC_AUTO_CONFIGURER_I_CONFIGURABLE_CONNECTOR_FACTORY_H
#define MOSAIC_AUTO_CONFIGURER_I_CONFIGURABLE_CONNECTOR_FACTORY_H

#include <memory>

#include <mosaic_rtc_core/configs_decl.h>

#include "i_configurable_connector.h"

namespace mosaic::auto_configurer {
class IConfigurableConnectorFactory {
  public:
    virtual ~IConfigurableConnectorFactory() = default;
    virtual std::string GetConnectorType() = 0;
    virtual std::shared_ptr<IConfigurableConnector> CreateConnector() = 0;
};
}  // namespace mosaic::auto_configurer

#endif  // MOSAIC_AUTO_CONFIGURER_I_CONFIGURABLE_CONNECTOR_FACTORY_H
