//
// Created by yhkim on 25. 6. 22.
//

#ifndef BA_GCS_RTC_SENDER_SIMPLE_SET_LOCAL_DESCRIPTION_OBSERVER_H
#define BA_GCS_RTC_SENDER_SIMPLE_SET_LOCAL_DESCRIPTION_OBSERVER_H

#include "api/jsep.h"
#include "rtc_sender/gcs_connector.h"

namespace rtc_sender::observers {
    class SimpleSetLocalDescriptionObserver : public webrtc::SetSessionDescriptionObserver {
    public:
        explicit SimpleSetLocalDescriptionObserver(
            const std::shared_ptr<PeerConnectionManager> &pc_manager,
            webrtc::SessionDescriptionInterface *desc)
            : pc_manager_(pc_manager), desc_(desc) {
        }

        void OnSuccess() override;

        void OnFailure(webrtc::RTCError error) override;

    private:
        std::shared_ptr<PeerConnectionManager> pc_manager_;
        webrtc::SessionDescriptionInterface *desc_;
    };
} // namespace rtc_sender::observers

#endif  // BA_GCS_RTC_SENDER_SIMPLE_SET_LOCAL_DESCRIPTION_OBSERVER_H
