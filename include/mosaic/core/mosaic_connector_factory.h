//
// Created by yhkim on 25. 7. 7.
//

#ifndef MOSAIC_RTC_CORE_MOSAIC_CONNECTOR_FACTORY_H
#define MOSAIC_RTC_CORE_MOSAIC_CONNECTOR_FACTORY_H

#include <memory>

#include <mosaic/configs_decl.h>
#include <mosaic/core/mosaic_connector.h>

namespace mosaic::core {
class MosaicConnectorFactory {
  public:
    explicit MosaicConnectorFactory(const std::shared_ptr<ServerConfig>& server_config)
        : server_config_(server_config) {}

    std::shared_ptr<MosaicConnector> Create() const;

  private:
    std::shared_ptr<ServerConfig> server_config_;
};
}  // namespace mosaic::core

#endif  // MOSAIC_RTC_CORE_MOSAIC_CONNECTOR_FACTORY_H
