//
// Created by yhkim on 25. 7. 1.
//

#ifndef BA_GCS_RTC_SENDER_A_DATA_CHANNEL_HANDLER_H
#define BA_GCS_RTC_SENDER_A_DATA_CHANNEL_HANDLER_H

#include <string>

#include "rtc_sender/logger/log.h"
#include "rtc_sender/webrtc_forward_decl.h"

namespace rtc_sender::handlers {
    class ADataChannelHandler {
    public:
        enum ChannelState { kConnecting, kOpen, kClosing, kClosed, kUnknown };

        explicit ADataChannelHandler(const std::string &label);

        virtual ~ADataChannelHandler() = default;

        std::string GetLabel() const;

        virtual void AfterCreate() {
        }

        virtual void OnMessage(const webrtc::DataBuffer &) {
            RTC_SENDER_LOG_ERROR("This DataChannel Handler is not Receivable!");
        }

        webrtc::scoped_refptr<webrtc::DataChannelInterface> CreateDataChannel(
            const webrtc::scoped_refptr<webrtc::PeerConnectionInterface> &peer_connection) const;

        void SetDataChannelInterface(const webrtc::scoped_refptr<webrtc::DataChannelInterface> &dc_interface) const;

        void RegisterDataChannelObserver() const;

        void RegisterDataChannelObserver(ADataChannelHandler *dc_handler) const;

        ChannelState GetState() const;

        void CloseDataChannel() const;

    private:
        class Impl;
        std::shared_ptr<Impl> pImpl;

        void Send(const webrtc::DataBuffer &buffer) const;

        friend class DataChannelSendable;

        Impl *GetImpl() const {
            return pImpl.get();
        }
    };
} // namespace rtc_sender::handlers

#endif  // BA_GCS_RTC_SENDER_A_DATA_CHANNEL_HANDLER_H
