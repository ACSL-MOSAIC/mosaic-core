//
// Created by yhkim on 12/25/25.
//

#ifndef MOSAIC_RTC_CORE_I_DATA_CHANNEL_HANDLER_H
#define MOSAIC_RTC_CORE_I_DATA_CHANNEL_HANDLER_H

#include <string>

#include <mosaic/logger/log.h>
#include <mosaic/rtc/webrtc_forward_decl.h>

namespace mosaic::handlers {
class IDataChannelHandler {
  public:
    enum ChannelState { kConnecting, kOpen, kClosing, kClosed, kUnknown };

    IDataChannelHandler() = default;

    virtual ~IDataChannelHandler() = default;

    virtual std::string GetLabel() const = 0;

    virtual void AfterCreate() {}

    virtual void OnMessage(const webrtc::DataBuffer&) {
        MOSAIC_LOG_ERROR("This DataChannel Handler is not Receivable!");
    }

    virtual void SetDataChannelInterface(
        const webrtc::scoped_refptr<webrtc::DataChannelInterface>& dc_interface) const = 0;

    virtual void RegisterDataChannelObserver(IDataChannelHandler* dc_handler) const = 0;

    virtual ChannelState GetState() const = 0;

    virtual void CloseDataChannel() const = 0;

  private:
    virtual void Send(const webrtc::DataBuffer& buffer) const = 0;

    friend class DataChannelSendable;
};
}  // namespace mosaic::handlers

#endif  // MOSAIC_RTC_CORE_I_DATA_CHANNEL_HANDLER_H
