//
// Created by yhkim on 25. 6. 5.
//

#ifndef CREATE_SDP_ANSWER_OBSERVER_H
#define CREATE_SDP_ANSWER_OBSERVER_H

#include <memory>

#include "api/jsep.h"
#include "rtc_sender/robot_webrtc_client.h"

namespace rtc_sender {
    namespace observers {
        class CreateSdpAnswerObserver : public webrtc::CreateSessionDescriptionObserver {
        public:
            explicit CreateSdpAnswerObserver(const std::shared_ptr<PeerConnectionManager> &peer_connection_manager)
                : peer_connection_manager_(peer_connection_manager) {
            }

            void OnSuccess(webrtc::SessionDescriptionInterface *desc) override;

            void OnFailure(webrtc::RTCError error) override;

        private:
            std::shared_ptr<PeerConnectionManager> peer_connection_manager_;
        };
    } // namespace observers
} // namespace rtc_sender

#endif  // CREATE_SDP_ANSWER_OBSERVER_H
