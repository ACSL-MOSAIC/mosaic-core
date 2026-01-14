//
// Created by yhkim on 12/28/25.
//

#include <mosaic_auto_configurer/connector/connector_resolver.h>

using namespace mosaic::auto_configurer;

ConnectorResolver ConnectorResolver::instance_;

ConnectorResolver& ConnectorResolver::GetInstance() {
    return instance_;
}

bool ConnectorResolver::IsSupportedType(const std::string& connector_type) const {
    return factory_map_.find(connector_type) != factory_map_.end();
}

std::shared_ptr<IConfigurableConnector> ConnectorResolver::GetConfigurableConnector(const std::string& connector_type) {
    auto it = factory_map_.find(connector_type);
    if (it != factory_map_.end()) {
        return it->second->CreateConnector();
    }
    return nullptr;
}
