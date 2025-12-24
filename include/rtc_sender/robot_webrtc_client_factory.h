//
// Created by yhkim on 25. 7. 7.
//

#ifndef ROBOT_WEBRTC_CLIENT_FACTORY_H
#define ROBOT_WEBRTC_CLIENT_FACTORY_H
#include <memory>

#include "robot_webrtc_client.h"
#include "signaling_config.h"

namespace rtc_sender {
    class RobotWebRTCClientFactory {
    public:
        RobotWebRTCClientFactory(const std::string &robot_id,
                                 const std::string &user_id,
                                 const std::shared_ptr<SignalingConfig> &signaling_config,
                                 const std::shared_ptr<IceConfig> &ice_config);

        RobotWebRTCClientFactory(const std::string &robot_id,
                                 const std::string &user_id,
                                 const std::string &ws_uri,
                                 const std::string &turn_url,
                                 const std::string &turn_username,
                                 const std::string &turn_credential);

        /**
         * @brief Creates a new RobotWebRTCClient instance
         *
         * This method creates a new instance of RobotWebRTCClient.
         *
         * @return std::shared_ptr<RobotWebRTCClient> Pointer to the created RobotWebRTCClient instance
         */
        std::shared_ptr<RobotWebRTCClient> Create();

    private:
        std::string robot_id_;
        std::string user_id_;
        std::shared_ptr<IceConfig> ice_config_;
        std::shared_ptr<SignalingConfig> signaling_config_;
    };
} // namespace rtc_sender

#endif  // ROBOT_WEBRTC_CLIENT_FACTORY_H
