//
// Created by yhkim on 12/25/25.
//

#ifndef BA_GCS_RTC_SENDER_I_DATA_CHANNEL_HANDLER_H
#define BA_GCS_RTC_SENDER_I_DATA_CHANNEL_HANDLER_H

#include <string>

#include <mosaic_rtc_core/logger/log.h>
#include <mosaic_rtc_core/webrtc_forward_decl.h>

namespace rtc_sender::handlers {
    class IDataChannelHandler {
    public:
        enum ChannelState { kConnecting, kOpen, kClosing, kClosed, kUnknown };

        explicit IDataChannelHandler();

        virtual ~IDataChannelHandler() = default;

        virtual std::string GetLabel() const;

        virtual void AfterCreate() {
        }

        virtual void OnMessage(const webrtc::DataBuffer &) {
            RTC_SENDER_LOG_ERROR("This DataChannel Handler is not Receivable!");
        }

        virtual void SetDataChannelInterface(
            const webrtc::scoped_refptr<webrtc::DataChannelInterface> &dc_interface) const;

        virtual void RegisterDataChannelObserver(IDataChannelHandler *dc_handler) const;

        virtual ChannelState GetState() const;

        virtual void CloseDataChannel() const;

    private:
        virtual void Send(const webrtc::DataBuffer &buffer) const;

        friend class DataChannelSendable;
    };
} // namespace rtc_sender::handlers

#endif //BA_GCS_RTC_SENDER_I_DATA_CHANNEL_HANDLER_H
