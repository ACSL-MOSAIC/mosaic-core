//
// Created by yhkim on 25. 6. 5.
//

#ifndef BA_GCS_RTC_SENDER_CREATE_SDP_ANSWER_OBSERVER_H
#define BA_GCS_RTC_SENDER_CREATE_SDP_ANSWER_OBSERVER_H

#include <memory>

#include <api/jsep.h>

#include <rtc_sender/gcs_connector.h>

namespace rtc_sender::observers {
    class CreateSdpAnswerObserver : public webrtc::CreateSessionDescriptionObserver {
    public:
        explicit CreateSdpAnswerObserver(const std::shared_ptr<PeerConnectionManager> &pc_manager)
            : pc_manager_(pc_manager) {
        }

        void OnSuccess(webrtc::SessionDescriptionInterface *desc) override;

        void OnFailure(webrtc::RTCError error) override;

    private:
        std::shared_ptr<PeerConnectionManager> pc_manager_;
    };
} // namespace rtc_sender::observers

#endif  // BA_GCS_RTC_SENDER_CREATE_SDP_ANSWER_OBSERVER_H
