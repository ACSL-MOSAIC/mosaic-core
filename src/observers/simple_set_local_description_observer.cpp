//
// Created by yhkim on 25. 6. 22.
//

#include <mosaic_rtc_core/logger/log.h>
#include <mosaic_rtc_core/observers/simple_set_local_description_observer.h>

using namespace mosaic::core_observers;

void SimpleSetLocalDescriptionObserver::OnSuccess() {
    if (!desc_) {
        MOSAIC_LOG_ERROR("No session description set for local description observer.");
        return;
    }
    pc_manager_->AfterSetLocalDescription(desc_);
}

void SimpleSetLocalDescriptionObserver::OnFailure(const webrtc::RTCError error) {
    MOSAIC_LOG_ERROR("Failed to set local description: {}", error.message());
}
