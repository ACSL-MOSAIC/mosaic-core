//
// Created by yhkim on 25. 7. 7.
//

#ifndef BA_GCS_RTC_SENDER_GCS_CONNECTOR_FACTORY_H
#define BA_GCS_RTC_SENDER_GCS_CONNECTOR_FACTORY_H

#include <memory>

#include <rtc_sender/gcs_connector.h>
#include <rtc_sender/signaling/signaling_config.h>

namespace rtc_sender {
    class GCSConnectorFactory {
    public:
        GCSConnectorFactory(const std::string &robot_id,
                            const std::string &user_id,
                            const std::shared_ptr<signaling::SignalingConfig> &signaling_config,
                            const std::shared_ptr<IceConfig> &ice_config);

        GCSConnectorFactory(const std::string &robot_id,
                            const std::string &user_id,
                            const std::string &ws_uri,
                            const std::string &turn_url,
                            const std::string &turn_username,
                            const std::string &turn_credential);

        std::shared_ptr<GCSConnector> Create();

    private:
        std::string robot_id_;
        std::string user_id_;
        std::shared_ptr<IceConfig> ice_config_;
        std::shared_ptr<signaling::SignalingConfig> signaling_config_;
    };
} // namespace rtc_sender

#endif  // BA_GCS_RTC_SENDER_GCS_CONNECTOR_FACTORY_H
