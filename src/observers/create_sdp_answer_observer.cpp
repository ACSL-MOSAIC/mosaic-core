//
// Created by yhkim on 25. 6. 5.
//

#include "rtc_sender/observers/create_sdp_answer_observer.h"

#include <iostream>

#include "rtc_sender/logger/log.h"
#include "rtc_sender/gcs_connector.h"
#include "rtc_sender/signaling_client.h"

using namespace rtc_sender::observers;

void CreateSdpAnswerObserver::OnSuccess(webrtc::SessionDescriptionInterface *desc) {
    pc_manager_->AfterCreateSdpAnswer(desc);
}

void CreateSdpAnswerObserver::OnFailure(webrtc::RTCError error) {
    RTC_SENDER_LOG_ERROR("Failed to create SDP answer: {}", error.message());
}
