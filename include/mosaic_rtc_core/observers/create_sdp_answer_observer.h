//
// Created by yhkim on 25. 6. 5.
//

#ifndef MOSAIC_RTC_CORE_CREATE_SDP_ANSWER_OBSERVER_H
#define MOSAIC_RTC_CORE_CREATE_SDP_ANSWER_OBSERVER_H

#include <memory>

#include <api/jsep.h>
#include <mosaic_rtc_core/core/mosaic_connector.h>

namespace mosaic::core_observers {
class CreateSdpAnswerObserver : public webrtc::CreateSessionDescriptionObserver {
  public:
    explicit CreateSdpAnswerObserver(const std::shared_ptr<core::PeerConnectionManager>& pc_manager)
        : pc_manager_(pc_manager) {}

    void OnSuccess(webrtc::SessionDescriptionInterface* desc) override;

    void OnFailure(webrtc::RTCError error) override;

  private:
    std::shared_ptr<core::PeerConnectionManager> pc_manager_;
};
}  // namespace mosaic::core_observers

#endif  // MOSAIC_RTC_CORE_CREATE_SDP_ANSWER_OBSERVER_H
