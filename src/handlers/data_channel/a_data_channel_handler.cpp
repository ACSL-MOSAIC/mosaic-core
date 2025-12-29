//
// Created by yhkim on 25. 7. 1.
//

#include <utility>

#include <api/peer_connection_interface.h>
#include <mosaic_rtc_core/handlers/data_channel/a_data_channel_handler.h>
#include <mosaic_rtc_core/handlers/data_channel/i_data_channel_receivable.h>
#include <mosaic_rtc_core/logger/log.h>
#include <mosaic_rtc_core/observers/data_channel_observer.h>

using namespace mosaic::handlers;

class ADataChannelHandler::Impl {
  public:
    explicit Impl(std::string label) : label_(std::move(label)) {}

    virtual ~Impl() = default;

    std::string GetLabel() const {
        return label_;
    }

    void SetDataChannelInterface(const webrtc::scoped_refptr<webrtc::DataChannelInterface>& dc_interface) {
        if (!dc_interface) {
            MOSAIC_LOG_ERROR("Attempted to set a null DataChannelInterface for label: {}", label_);
            throw std::runtime_error("DataChannelInterface cannot be null");
        }
        dc_interface_ = dc_interface;
    }

    void RegisterDataChannelObserver(IDataChannelHandler* dc_handler) {
        if (!dc_interface_) {
            MOSAIC_LOG_ERROR("DataChannelInterface cannot be set for label: {}", label_);
            throw std::runtime_error("DataChannelInterface is not set");
        }

        std::function<void(const webrtc::DataBuffer&)> onMessageLambda;

        if (auto receivable_ptr = static_cast<IDataChannelReceivable*>(dc_handler)) {
            MOSAIC_LOG_DEBUG("Receivable DataChannel Detected!");
            onMessageLambda = [receivable_ptr](const webrtc::DataBuffer& buffer) { receivable_ptr->OnMessage(buffer); };
        } else {
            onMessageLambda = [this](const webrtc::DataBuffer&) {};
        }

        observer_ = std::make_shared<core_observers::DataChannelObserver>(onMessageLambda);
        dc_interface_->RegisterObserver(observer_.get());
        MOSAIC_LOG_INFO("DataChannelObserver registered for label (Receivable): {}", label_);
    }

    ChannelState GetState() const {
        if (!dc_interface_) {
            return kUnknown;
        }

        switch (dc_interface_->state()) {
            case webrtc::DataChannelInterface::kConnecting:
                return kConnecting;
            case webrtc::DataChannelInterface::kOpen:
                return kOpen;
            case webrtc::DataChannelInterface::kClosing:
                return kClosing;
            case webrtc::DataChannelInterface::kClosed:
                return kClosed;
            default:
                return kUnknown;
        }
    }

    void CloseDataChannel() {
        if (dc_interface_) {
            dc_interface_->Close();
            dc_interface_ = nullptr;
        } else {
            MOSAIC_LOG_WARNING("Attempted to close a null DataChannelInterface for label: {}", label_);
        }
    }

  private:
    std::string label_;
    mutable webrtc::scoped_refptr<webrtc::DataChannelInterface> dc_interface_;
    mutable std::shared_ptr<core_observers::DataChannelObserver> observer_;

    void Send(const webrtc::DataBuffer& buffer) const {
        dc_interface_->Send(buffer);
    }

    friend class ADataChannelHandler;
    friend class DataChannelSendable;
};

ADataChannelHandler::ADataChannelHandler(const std::string& label) {
    pImpl = std::make_shared<Impl>(label);
}

std::string ADataChannelHandler::GetLabel() const {
    return pImpl->GetLabel();
}

void ADataChannelHandler::SetDataChannelInterface(
    const webrtc::scoped_refptr<webrtc::DataChannelInterface>& dc_interface) const {
    return pImpl->SetDataChannelInterface(dc_interface);
}

void ADataChannelHandler::RegisterDataChannelObserver(IDataChannelHandler* dc_handler) const {
    pImpl->RegisterDataChannelObserver(dc_handler);
}

ADataChannelHandler::ChannelState ADataChannelHandler::GetState() const {
    return pImpl->GetState();
}

void ADataChannelHandler::CloseDataChannel() const {
    pImpl->CloseDataChannel();
}

void ADataChannelHandler::Send(const webrtc::DataBuffer& buffer) const {
    pImpl->Send(buffer);
}
