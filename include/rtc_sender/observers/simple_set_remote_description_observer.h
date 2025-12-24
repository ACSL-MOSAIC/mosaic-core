//
// Created by yhkim on 25. 6. 22.
//

#ifndef SIMPLE_SET_SESSION_DESCRIPTION_OBSERVER_H
#define SIMPLE_SET_SESSION_DESCRIPTION_OBSERVER_H

#include <memory>

#include "api/jsep.h"
#include "rtc_sender/robot_webrtc_client.h"

namespace rtc_sender {
    namespace observers {
        class SimpleSetRemoteDescriptionObserver : public webrtc::SetSessionDescriptionObserver {
        public:
            explicit SimpleSetRemoteDescriptionObserver(
                const std::shared_ptr<PeerConnectionManager> &peer_connection_manager)
                : peer_connection_manager_(peer_connection_manager) {
            }

            void OnSuccess() override;

            void OnFailure(webrtc::RTCError error) override;

        private:
            std::shared_ptr<PeerConnectionManager> peer_connection_manager_;
        };
    } // namespace observers
} // namespace rtc_sender

#endif  // SIMPLE_SET_SESSION_DESCRIPTION_OBSERVER_H
