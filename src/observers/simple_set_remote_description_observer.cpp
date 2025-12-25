//
// Created by yhkim on 25. 6. 22.
//

#include "rtc_sender/observers/simple_set_remote_description_observer.h"

#include "rtc_sender/logger/log.h"

using namespace rtc_sender::observers;

void SimpleSetRemoteDescriptionObserver::OnSuccess() {
    RTC_SENDER_LOG_INFO("Set remote description with SDP offer Success");
    pc_manager_->AfterSetSessionDescription();
}

void SimpleSetRemoteDescriptionObserver::OnFailure(webrtc::RTCError error) {
    RTC_SENDER_LOG_ERROR("Failed to set session description: {}", error.message());
}
