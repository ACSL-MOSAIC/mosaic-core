//
// Created by yhkim on 25. 6. 22.
//

#ifndef SIMPLE_SET_LOCAL_DESCRIPTION_OBSERVER_H
#define SIMPLE_SET_LOCAL_DESCRIPTION_OBSERVER_H

#include "api/jsep.h"
#include "rtc_sender/robot_webrtc_client.h"

namespace rtc_sender {
    namespace observers {
        class SimpleSetLocalDescriptionObserver : public webrtc::SetSessionDescriptionObserver {
        public:
            explicit SimpleSetLocalDescriptionObserver(
                const std::shared_ptr<PeerConnectionManager> &peer_connection_manager,
                webrtc::SessionDescriptionInterface *desc)
                : peer_connection_manager_(peer_connection_manager), desc_(desc) {
            }

            void OnSuccess() override;

            void OnFailure(webrtc::RTCError error) override;

        private:
            std::shared_ptr<PeerConnectionManager> peer_connection_manager_;
            webrtc::SessionDescriptionInterface *desc_;
        };
    } // namespace observers
} // namespace rtc_sender

#endif  // SIMPLE_SET_LOCAL_DESCRIPTION_OBSERVER_H
