//
// Created by yhkim on 25. 7. 1.
//

#ifndef BA_GCS_RTC_SENDER_A_DATA_CHANNEL_HANDLER_H
#define BA_GCS_RTC_SENDER_A_DATA_CHANNEL_HANDLER_H

#include "mosaic_rtc_core/handlers/data_channel/i_data_channel_handler.h"

namespace rtc_sender::handlers {
    class ADataChannelHandler : public IDataChannelHandler {
    public:
        explicit ADataChannelHandler(const std::string &label);

        ~ADataChannelHandler() override = default;

        [[nodiscard]] std::string GetLabel() const override;

        void OnMessage(const webrtc::DataBuffer &) override {
            RTC_SENDER_LOG_ERROR("This DataChannel Handler is not Receivable!");
        }

        void SetDataChannelInterface(
            const webrtc::scoped_refptr<webrtc::DataChannelInterface> &dc_interface) const override;

        void RegisterDataChannelObserver(IDataChannelHandler *dc_handler) const override;

        [[nodiscard]] ChannelState GetState() const override;

        void CloseDataChannel() const override;

    private:
        class Impl;
        std::shared_ptr<Impl> pImpl;

        void Send(const webrtc::DataBuffer &buffer) const override;

        friend class DataChannelSendable;

        [[nodiscard]] Impl *GetImpl() const {
            return pImpl.get();
        }
    };
} // namespace rtc_sender::handlers

#endif  // BA_GCS_RTC_SENDER_A_DATA_CHANNEL_HANDLER_H
