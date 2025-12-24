//
// Created by yhkim on 25. 6. 22.
//

#include "rtc_sender/observers/simple_set_local_description_observer.h"

#include "rtc_sender/logger/log.h"

using namespace rtc_sender::observers;

void SimpleSetLocalDescriptionObserver::OnSuccess() {
    if (!desc_) {
        RTC_SENDER_LOG_ERROR("No session description set for local description observer.");
        return;
    }
    peer_connection_manager_->AfterSetLocalDescription(desc_);
}

void SimpleSetLocalDescriptionObserver::OnFailure(webrtc::RTCError error) {
    RTC_SENDER_LOG_ERROR("Failed to set local description: {}", error.message());
}
