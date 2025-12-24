//
// Created by yhkim on 25. 7. 2.
//

#ifndef I_DATA_CHANNEL_RECEIVABLE_H
#define I_DATA_CHANNEL_RECEIVABLE_H

#include "a_data_channel_handler.h"
#include "rtc_sender/webrtc_forward_decl.h"

namespace Json {
    class Value;
}

namespace rtc_sender {
    namespace handlers {
        class IDataChannelReceivable : public ADataChannelHandler {
        public:
            IDataChannelReceivable(const std::string &label, bool need_to_create)
                : ADataChannelHandler(label, need_to_create) {
            }

            ~IDataChannelReceivable() override = default;

            Json::Value ConvertDataBufferToJson(const webrtc::DataBuffer &buffer);

            void OnMessage(const webrtc::DataBuffer &buffer) override = 0;
        };
    } // namespace handlers
} // namespace rtc_sender

#endif  // I_DATA_CHANNEL_RECEIVABLE_H
