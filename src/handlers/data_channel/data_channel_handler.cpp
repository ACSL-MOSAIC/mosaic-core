//
// Created by yhkim on 25. 7. 1.
//

#include "mosaic/handlers/data_channel/data_channel_handler.hpp"

#include <mutex>
#include <queue>
#include <utility>

#include <api/data_channel_interface.h>
#include <api/peer_connection_interface.h>

#include "mosaic/handlers/data_channel/data_channel_receivable.hpp"
#include "mosaic/logger/log.hpp"

namespace mosaic::handlers {

static constexpr uint64_t kBufferedAmountHighThreshold = 64 * 1024;  // 64KB: stop sending above this
static constexpr uint64_t kBufferedAmountLowThreshold = 16 * 1024;   // 16KB: resume sending below this

class DataChannelObserver final : public webrtc::DataChannelObserver {
  public:
    explicit DataChannelObserver(std::function<void(const webrtc::DataBuffer&)> onMessageCallback,
                                 std::function<void()> onBufferedAmountChangedCallback)
        : onMessageCallback_(std::move(onMessageCallback)),
          onBufferedAmountChangedCallback_(std::move(onBufferedAmountChangedCallback)) {}

    void OnStateChange() override {
        // Handle state change events here
    }

    void OnMessage(const webrtc::DataBuffer& buffer) override {
        // Handle incoming messages here
        onMessageCallback_(buffer);
    }

    void OnBufferedAmountChange(uint64_t sent_data_size) override {
        // MOSAIC_LOG_INFO("DataChannel Buffered amount changed: {}", sent_data_size);
        if (onBufferedAmountChangedCallback_) {
            onBufferedAmountChangedCallback_();
        }
    }

    bool IsOkToCallOnTheNetworkThread() override {
        return true;  // Allow callbacks on the network thread
    }

  private:
    std::function<void(const webrtc::DataBuffer&)> onMessageCallback_;
    std::function<void()> onBufferedAmountChangedCallback_;
};

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

        if (auto receivable_ptr = dynamic_cast<IDataChannelReceivable*>(dc_handler)) {
            MOSAIC_LOG_DEBUG("Receivable DataChannel Detected!");
            onMessageLambda = [receivable_ptr](const webrtc::DataBuffer& buffer) { receivable_ptr->OnMessage(buffer); };
        } else {
            onMessageLambda = [](const webrtc::DataBuffer&) {};
        }

        std::function<void()> onBufferedAmountChangedLambda = [this]() {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            // MOSAIC_LOG_INFO("DataChannel buffer changed, resuming send queue for label: {}", label_);
            ProcessQueue();
        };

        observer_ = std::make_shared<DataChannelObserver>(onMessageLambda, onBufferedAmountChangedLambda);
        dc_interface_->RegisterObserver(observer_.get());
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
    mutable std::shared_ptr<DataChannelObserver> observer_;
    mutable std::queue<webrtc::DataBuffer> send_queue_;
    mutable std::mutex queue_mutex_;
    mutable bool is_sending_ = false;

    void Send(const webrtc::DataBuffer& buffer) const {
        dc_interface_->Send(buffer);
    }

    void SendAsync(const webrtc::DataBuffer& buffer) const {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        send_queue_.push(buffer);
        ProcessQueue();
    }

    void ProcessQueue() const {
        if (is_sending_ || send_queue_.empty()) {
            return;
        }

        is_sending_ = true;
        const auto buffer = send_queue_.front();
        if (buffer.size() > webrtc::DataChannelInterface::MaxSendQueueSize() - dc_interface_->buffered_amount()) {
            MOSAIC_LOG_INFO(
                "DataChannel buffer high ({}B), pausing send for label: {}", dc_interface_->buffered_amount(), label_);
            return;
        }
        send_queue_.pop();

        MOSAIC_LOG_DEBUG("Buffer Size: {}", buffer.size());

        dc_interface_->SendAsync(buffer, [this](webrtc::RTCError error) {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            is_sending_ = false;

            if (!error.ok()) {
                MOSAIC_LOG_ERROR("SendAsync failed for label: {}, type: {}, detail: {}, error: {}",
                                 label_,
                                 static_cast<int>(error.type()),
                                 static_cast<int>(error.error_detail()),
                                 error.message());
            }

            ProcessQueue();
        });
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

void ADataChannelHandler::RegisterDataChannelObserver() const {
    pImpl->RegisterDataChannelObserver(const_cast<ADataChannelHandler*>(this));
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

void ADataChannelHandler::SendAsync(const webrtc::DataBuffer& buffer) const {
    pImpl->SendAsync(buffer);
}

}  // namespace mosaic::handlers
