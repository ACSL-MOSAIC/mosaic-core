//
// Created by yhkim on 25. 6. 4.
//

#ifndef DATACHANNELOBSERVER_H
#define DATACHANNELOBSERVER_H

#include "api/data_channel_interface.h"

namespace rtc_sender {
    namespace observers {
        class DataChannelObserver final : public webrtc::DataChannelObserver {
        public:
            explicit DataChannelObserver(std::function<void(const webrtc::DataBuffer &)> onMessageCallback);

            void OnStateChange() override;

            void OnMessage(const webrtc::DataBuffer &buffer) override;

            void OnBufferedAmountChange(uint64_t sent_data_size) override;

            bool IsOkToCallOnTheNetworkThread() override {
                return true; // Allow callbacks on the network thread
            }

        private:
            std::function<void(const webrtc::DataBuffer &)> onMessageCallback_;
        };
    } // namespace observers
} // namespace rtc_sender

#endif  // DATACHANNELOBSERVER_H
