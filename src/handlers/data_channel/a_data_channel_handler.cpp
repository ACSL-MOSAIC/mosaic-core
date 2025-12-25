//
// Created by yhkim on 25. 7. 1.
//

#include "rtc_sender/handlers/data_channel/a_data_channel_handler.h"

#include <rtc_sender/handlers/data_channel/data_channel_receivable.h>

#include "api/peer_connection_interface.h"
#include "rtc_sender/handlers/data_channel/i_data_channel_receivable.h"
#include "rtc_sender/logger/log.h"
#include "rtc_sender/observers/data_channel_observer.h"

using namespace rtc_sender::handlers;

class ADataChannelHandler::Impl {
public:
    explicit Impl(const std::string &label) : label_(label) {
    }

    virtual ~Impl() = default;

    std::string GetLabel() const {
        return label_;
    }

    webrtc::scoped_refptr<webrtc::DataChannelInterface> CreateDataChannel(
        const webrtc::scoped_refptr<webrtc::PeerConnectionInterface> &peer_connection) {
        webrtc::DataChannelInit config;
        config.ordered = true;

        auto error_or_dc = peer_connection->CreateDataChannelOrError(label_, &config);
        if (!error_or_dc.ok()) {
            RTC_SENDER_LOG_ERROR(
                "Failed to create DataChannel with label: {}, error: {}", label_, error_or_dc.error().message());
            throw std::runtime_error("DataChannel creation failed: " + std::string(error_or_dc.error().message()));
        }

        auto dc = error_or_dc.value();
        SetDataChannelInterface(dc);
        RegisterDataChannelObserver();

        return dc;
    }

    void SetDataChannelInterface(const webrtc::scoped_refptr<webrtc::DataChannelInterface> &dc_interface) {
        if (!dc_interface) {
            RTC_SENDER_LOG_ERROR("Attempted to set a null DataChannelInterface for label: {}", label_);
            throw std::runtime_error("DataChannelInterface cannot be null");
        }
        dc_interface_ = dc_interface;
    }

    void RegisterDataChannelObserver() {
        if (!dc_interface_) {
            RTC_SENDER_LOG_ERROR("DataChannelInterface cannot be set for label: {}", label_);
            throw std::runtime_error("DataChannelInterface is not set");
        }

        observer_ = std::make_shared<observers::DataChannelObserver>([this](const webrtc::DataBuffer &) {
        });
        dc_interface_->RegisterObserver(observer_.get());
        RTC_SENDER_LOG_INFO("DataChannelObserver registered for label (Sendable): {}", label_);
    }

    void RegisterDataChannelObserver(ADataChannelHandler *dc_handler) {
        if (!dc_interface_) {
            RTC_SENDER_LOG_ERROR("DataChannelInterface cannot be set for label: {}", label_);
            throw std::runtime_error("DataChannelInterface is not set");
        }

        std::function<void(const webrtc::DataBuffer &)> onMessageLambda;

        if (auto receivable_ptr = static_cast<IDataChannelReceivable *>(dc_handler)) {
            RTC_SENDER_LOG_DEBUG("Receivable DataChannel Detected!");
            onMessageLambda = [receivable_ptr](const webrtc::DataBuffer &buffer) { receivable_ptr->OnMessage(buffer); };
        } else {
            onMessageLambda = [this](const webrtc::DataBuffer &) {
            };
        }

        observer_ = std::make_shared<observers::DataChannelObserver>(onMessageLambda);
        dc_interface_->RegisterObserver(observer_.get());
        RTC_SENDER_LOG_INFO("DataChannelObserver registered for label (Receivable): {}", label_);
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
            RTC_SENDER_LOG_WARNING("Attempted to close a null DataChannelInterface for label: {}", label_);
        }
    }

private:
    std::string label_;
    mutable webrtc::scoped_refptr<webrtc::DataChannelInterface> dc_interface_;
    mutable std::shared_ptr<observers::DataChannelObserver> observer_;

    void Send(const webrtc::DataBuffer &buffer) const {
        dc_interface_->Send(buffer);
    }

    friend class ADataChannelHandler;
    friend class DataChannelSendable;
};

ADataChannelHandler::ADataChannelHandler(const std::string &label) {
    pImpl = std::make_shared<Impl>(label);
}

std::string ADataChannelHandler::GetLabel() const {
    return pImpl->GetLabel();
}

webrtc::scoped_refptr<webrtc::DataChannelInterface> ADataChannelHandler::CreateDataChannel(
    const webrtc::scoped_refptr<webrtc::PeerConnectionInterface> &peer_connection) const {
    return pImpl->CreateDataChannel(peer_connection);
}

void ADataChannelHandler::SetDataChannelInterface(
    const webrtc::scoped_refptr<webrtc::DataChannelInterface> &dc_interface) const {
    return pImpl->SetDataChannelInterface(dc_interface);
}

void ADataChannelHandler::RegisterDataChannelObserver() const {
    pImpl->RegisterDataChannelObserver();
}

void ADataChannelHandler::RegisterDataChannelObserver(ADataChannelHandler *dc_handler) const {
    pImpl->RegisterDataChannelObserver(dc_handler);
}

ADataChannelHandler::ChannelState ADataChannelHandler::GetState() const {
    return pImpl->GetState();
}

void ADataChannelHandler::CloseDataChannel() const {
    pImpl->CloseDataChannel();
}

void ADataChannelHandler::Send(const webrtc::DataBuffer &buffer) const {
    pImpl->Send(buffer);
}
