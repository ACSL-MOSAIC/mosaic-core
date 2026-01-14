//
// Created by yhkim on 25. 6. 5.
//

#include <mosaic/core/mosaic_connector.h>
#include <mosaic/logger/log.h>
#include <mosaic/observers/create_sdp_answer_observer.h>

using namespace mosaic::core_observers;

void CreateSdpAnswerObserver::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
    pc_manager_->AfterCreateSdpAnswer(desc);
}

void CreateSdpAnswerObserver::OnFailure(const webrtc::RTCError error) {
    MOSAIC_LOG_ERROR("Failed to create SDP answer: {}", error.message());
}
