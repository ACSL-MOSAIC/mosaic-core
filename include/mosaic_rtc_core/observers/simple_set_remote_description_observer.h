//
// Created by yhkim on 25. 6. 22.
//

#ifndef BA_GCS_RTC_SENDER_SIMPLE_SET_SESSION_DESCRIPTION_OBSERVER_H
#define BA_GCS_RTC_SENDER_SIMPLE_SET_SESSION_DESCRIPTION_OBSERVER_H

#include <memory>

#include <api/jsep.h>
#include <mosaic_rtc_core/gcs_connector.h>

namespace rtc_sender::observers {
    class SimpleSetRemoteDescriptionObserver : public webrtc::SetSessionDescriptionObserver {
    public:
        explicit SimpleSetRemoteDescriptionObserver(
            const std::shared_ptr<PeerConnectionManager> &pc_manager)
            : pc_manager_(pc_manager) {
        }

        void OnSuccess() override;

        void OnFailure(webrtc::RTCError error) override;

    private:
        std::shared_ptr<PeerConnectionManager> pc_manager_;
    };
} // namespace rtc_sender::observers

#endif  // BA_GCS_RTC_SENDER_SIMPLE_SET_SESSION_DESCRIPTION_OBSERVER_H
