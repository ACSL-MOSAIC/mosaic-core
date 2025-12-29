//
// Created by yhkim on 25. 7. 7.
//

#ifndef MOSAIC_RTC_CORE_GCS_CONNECTOR_FACTORY_H
#define MOSAIC_RTC_CORE_GCS_CONNECTOR_FACTORY_H

#include <memory>

#include <mosaic_rtc_core/mosaic_connector.h>
#include <mosaic_rtc_core/signaling/signaling_config.h>

namespace mosaic::core {
class MosaicConnectorFactory {
  public:
    MosaicConnectorFactory(const std::string& robot_id,
                           const std::string& user_id,
                           const std::shared_ptr<core_signaling::SignalingConfig>& signaling_config,
                           const std::shared_ptr<IceConfig>& ice_config);

    MosaicConnectorFactory(const std::string& robot_id,
                           const std::string& user_id,
                           const std::string& ws_uri,
                           const std::string& turn_url,
                           const std::string& turn_username,
                           const std::string& turn_credential);

    std::shared_ptr<MosaicConnector> Create();

  private:
    std::string robot_id_;
    std::string user_id_;
    std::shared_ptr<IceConfig> ice_config_;
    std::shared_ptr<core_signaling::SignalingConfig> signaling_config_;
};
}  // namespace mosaic::core

#endif  // MOSAIC_RTC_CORE_GCS_CONNECTOR_FACTORY_H
