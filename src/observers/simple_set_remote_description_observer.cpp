//
// Created by yhkim on 25. 6. 22.
//

#include <mosaic_rtc_core/logger/log.h>
#include <mosaic_rtc_core/observers/simple_set_remote_description_observer.h>

using namespace mosaic::core_observers;

void SimpleSetRemoteDescriptionObserver::OnSuccess() {
    MOSAIC_LOG_INFO("Set remote description with SDP offer Success");
    pc_manager_->AfterSetSessionDescription();
}

void SimpleSetRemoteDescriptionObserver::OnFailure(const webrtc::RTCError error) {
    MOSAIC_LOG_ERROR("Failed to set session description: {}", error.message());
}
