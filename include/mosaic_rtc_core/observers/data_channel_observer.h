//
// Created by yhkim on 25. 6. 4.
//

#ifndef MOSAIC_RTC_CORE_DATA_CHANNEL_OBSERVER_H
#define MOSAIC_RTC_CORE_DATA_CHANNEL_OBSERVER_H

#include <api/data_channel_interface.h>

namespace mosaic::core_observers {
class DataChannelObserver final : public webrtc::DataChannelObserver {
  public:
    explicit DataChannelObserver(std::function<void(const webrtc::DataBuffer&)> onMessageCallback)
        : onMessageCallback_(std::move(onMessageCallback)) {}

    void OnStateChange() override;

    void OnMessage(const webrtc::DataBuffer& buffer) override;

    void OnBufferedAmountChange(uint64_t sent_data_size) override;

    bool IsOkToCallOnTheNetworkThread() override {
        return true;  // Allow callbacks on the network thread
    }

  private:
    std::function<void(const webrtc::DataBuffer&)> onMessageCallback_;
};
}  // namespace mosaic::core_observers

#endif  // MOSAIC_RTC_CORE_DATA_CHANNEL_OBSERVER_H
