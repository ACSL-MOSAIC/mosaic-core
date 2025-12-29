//
// Created by yhkim on 25. 6. 22.
//

#ifndef MOSAIC_RTC_CORE_SIMPLE_SET_LOCAL_DESCRIPTION_OBSERVER_H
#define MOSAIC_RTC_CORE_SIMPLE_SET_LOCAL_DESCRIPTION_OBSERVER_H

#include <api/jsep.h>
#include <mosaic_rtc_core/mosaic_connector.h>

namespace mosaic::core_observers {
class SimpleSetLocalDescriptionObserver : public webrtc::SetSessionDescriptionObserver {
  public:
    explicit SimpleSetLocalDescriptionObserver(const std::shared_ptr<core::PeerConnectionManager>& pc_manager,
                                               webrtc::SessionDescriptionInterface* desc)
        : pc_manager_(pc_manager), desc_(desc) {}

    void OnSuccess() override;

    void OnFailure(webrtc::RTCError error) override;

  private:
    std::shared_ptr<core::PeerConnectionManager> pc_manager_;
    webrtc::SessionDescriptionInterface* desc_;
};
}  // namespace mosaic::core_observers

#endif  // MOSAIC_RTC_CORE_SIMPLE_SET_LOCAL_DESCRIPTION_OBSERVER_H
