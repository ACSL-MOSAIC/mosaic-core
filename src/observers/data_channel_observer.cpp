//
// Created by yhkim on 25. 6. 4.
//

#include <utility>

#include <mosaic_rtc_core/logger/log.h>
#include <mosaic_rtc_core/observers/data_channel_observer.h>

using namespace mosaic::core_observers;

DataChannelObserver::DataChannelObserver(std::function<void(const webrtc::DataBuffer&)> onMessageCallback)
    : onMessageCallback_(std::move(onMessageCallback)) {}

void DataChannelObserver::OnStateChange() {
    // Handle state change events here
}

void DataChannelObserver::OnMessage(const webrtc::DataBuffer& buffer) {
    // Handle incoming messages here
    onMessageCallback_(buffer);
}

void DataChannelObserver::OnBufferedAmountChange(uint64_t sent_data_size) {
    // Handle changes in buffered amount
    MOSAIC_LOG_DEBUG("DataChannel Buffered amount changed: {}", sent_data_size);
}
