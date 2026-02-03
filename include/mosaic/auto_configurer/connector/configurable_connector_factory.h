//
// Created by yhkim on 12/31/25.
//

#ifndef MOSAIC_AUTO_CONFIGURER_CONFIGURABLE_CONNECTOR_FACTORY_H
#define MOSAIC_AUTO_CONFIGURER_CONFIGURABLE_CONNECTOR_FACTORY_H

#include <memory>
#include <type_traits>

#include "i_configurable_connector.h"

namespace mosaic::auto_configurer {
class IConfigurableConnectorFactory {
  public:
    virtual ~IConfigurableConnectorFactory() = default;
    virtual std::string GetConnectorType() = 0;
    virtual std::shared_ptr<IConfigurableConnector> CreateConnector() = 0;
};

template <typename T>
class ConfigurableConnectorFactory : public IConfigurableConnectorFactory {
    static_assert(std::is_base_of_v<IConfigurableConnector, T>, "T must inherit from IConfigurableConnector");

  public:
    std::string GetConnectorType() override {
        const auto temp_instance = std::make_shared<T>();
        return temp_instance->GetConnectorType();
    }

    std::shared_ptr<IConfigurableConnector> CreateConnector() override {
        return std::make_shared<T>();
    }
};
}  // namespace mosaic::auto_configurer

#endif  // MOSAIC_AUTO_CONFIGURER_CONFIGURABLE_CONNECTOR_FACTORY_H
