//
// Created by yhkim on 12/28/25.
//

#ifndef MOSAIC_AUTO_CONFIGURER_CONNECTOR_RESOLVER_H
#define MOSAIC_AUTO_CONFIGURER_CONNECTOR_RESOLVER_H

#include <memory>
#include <string>
#include <unordered_map>

#include "configurable_connector_factory.h"
#include "i_configurable_connector.h"

namespace mosaic::auto_configurer {
class ConnectorResolver {
  public:
    static ConnectorResolver& GetInstance();

    bool IsSupportedType(const std::string& connector_type) const;

    std::shared_ptr<IConfigurableConnector> GetConfigurableConnector(const std::string& connector_type);

    template <typename T>
    void RegisterConfigurableConnector() {
        static_assert(std::is_base_of_v<IConfigurableConnector, T>, "T must inherit from IConfigurableConnector");
        auto factory = std::make_shared<ConfigurableConnectorFactory<T>>();
        factory_map_[factory->GetConnectorType()] = factory;
    }

    void RegisterConfigurableConnector(const std::shared_ptr<IConfigurableConnectorFactory>& factory) {
        factory_map_[factory->GetConnectorType()] = factory;
    }

    ConnectorResolver(const ConnectorResolver&) = delete;

    ConnectorResolver& operator=(const ConnectorResolver&) = delete;

  private:
    std::unordered_map<std::string, std::shared_ptr<IConfigurableConnectorFactory>> factory_map_;

    ConnectorResolver() = default;
};
}  // namespace mosaic::auto_configurer

#endif  // MOSAIC_AUTO_CONFIGURER_CONNECTOR_RESOLVER_H
