//
// Created by yhkim on 25. 6. 5.
//

#include <iostream>

#include <mosaic_rtc_core/core/mosaic_connector.h>
#include <mosaic_rtc_core/logger/log.h>
#include <mosaic_rtc_core/observers/create_sdp_answer_observer.h>

using namespace mosaic::core_observers;

void CreateSdpAnswerObserver::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
    pc_manager_->AfterCreateSdpAnswer(desc);
}

void CreateSdpAnswerObserver::OnFailure(const webrtc::RTCError error) {
    MOSAIC_LOG_ERROR("Failed to create SDP answer: {}", error.message());
}
