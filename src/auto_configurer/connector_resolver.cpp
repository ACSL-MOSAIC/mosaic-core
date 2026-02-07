//
// Created by yhkim on 12/28/25.
//

#include <mosaic/auto_configurer/connector/connector_resolver.h>

using namespace mosaic::auto_configurer;

ConnectorResolver& ConnectorResolver::GetInstance() {
    static ConnectorResolver instance;
    return instance;
}

bool ConnectorResolver::IsSupportedType(const std::string& connector_type) const {
    return factory_map_.find(connector_type) != factory_map_.end();
}

std::shared_ptr<IConfigurableConnector> ConnectorResolver::GetConfigurableConnector(const std::string& connector_type) {
    auto it = factory_map_.find(connector_type);
    if (it != factory_map_.end()) {
        return it->second->CreateConnector();
    }
    MOSAIC_LOG_ERROR("No factory found for connector type: {}", connector_type);
    return nullptr;
}
