//
// Created by yhkim on 25. 6. 22.
//

#ifndef MOSAIC_RTC_CORE_SIMPLE_SET_SESSION_DESCRIPTION_OBSERVER_H
#define MOSAIC_RTC_CORE_SIMPLE_SET_SESSION_DESCRIPTION_OBSERVER_H

#include <memory>

#include <api/jsep.h>
#include <mosaic_rtc_core/mosaic_connector.h>

namespace mosaic::core_observers {
class SimpleSetRemoteDescriptionObserver : public webrtc::SetSessionDescriptionObserver {
  public:
    explicit SimpleSetRemoteDescriptionObserver(const std::shared_ptr<core::PeerConnectionManager>& pc_manager)
        : pc_manager_(pc_manager) {}

    void OnSuccess() override;

    void OnFailure(webrtc::RTCError error) override;

  private:
    std::shared_ptr<core::PeerConnectionManager> pc_manager_;
};
}  // namespace mosaic::core_observers

#endif  // MOSAIC_RTC_CORE_SIMPLE_SET_SESSION_DESCRIPTION_OBSERVER_H
