//
// Created by yhkim on 25. 7. 1.
//

#ifndef MOSAIC_CORE_A_DATA_CHANNEL_HANDLER_HPP
#define MOSAIC_CORE_A_DATA_CHANNEL_HANDLER_HPP

#include <string>

#include "../../logger/log.hpp"
#include "../../rtc/webrtc_forward_decl.hpp"

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

    virtual void RegisterDataChannelObserver() const = 0;

    virtual ChannelState GetState() const = 0;

    virtual void CloseDataChannel() const = 0;

  private:
    virtual void Send(const webrtc::DataBuffer& buffer) const = 0;

    virtual void SendAsync(const webrtc::DataBuffer& buffer) const = 0;

    friend class DataChannelSendable;
};

class ADataChannelHandler : public IDataChannelHandler {
  public:
    explicit ADataChannelHandler(const std::string& label);

    ~ADataChannelHandler() override = default;

    [[nodiscard]] std::string GetLabel() const override;

    void OnMessage(const webrtc::DataBuffer&) override {
        MOSAIC_LOG_ERROR("This DataChannel Handler is not Receivable!");
    }

    void SetDataChannelInterface(
        const webrtc::scoped_refptr<webrtc::DataChannelInterface>& dc_interface) const override;

    void RegisterDataChannelObserver() const override;

    [[nodiscard]] ChannelState GetState() const override;

    void CloseDataChannel() const override;

  private:
    class Impl;
    std::shared_ptr<Impl> pImpl;

    void Send(const webrtc::DataBuffer& buffer) const override;

    void SendAsync(const webrtc::DataBuffer& buffer) const override;

    friend class DataChannelSendable;

    [[nodiscard]] Impl* GetImpl() const {
        return pImpl.get();
    }
};
}  // namespace mosaic::handlers

#endif  // MOSAIC_CORE_A_DATA_CHANNEL_HANDLER_HPP
