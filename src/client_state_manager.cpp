//
// Created by yhkim on 25. 6. 26.
//

#include "rtc_sender/client_state_manager.h"

#include <mutex>

#include "rtc_sender/logger/log.h"

using namespace rtc_sender;

ClientStateManager::ClientStateManager(const std::shared_ptr<signaling::ISignalingClient> &signaling_client)
    : state_(INITIALIZING), signaling_client_(signaling_client) {
}

void ClientStateManager::SetState(const State new_state) {
    std::unique_lock lock(mutex_);
    LogState(state_, new_state);
    state_ = new_state;
    SendStateToSignalingServer();
}

ClientStateManager::State ClientStateManager::GetState() const {
    std::shared_lock lock(mutex_);
    return state_;
}

std::string ClientStateManager::GetStateString() const {
    std::shared_lock lock(mutex_);
    return StateToString(state_);
}

bool ClientStateManager::IsState(const State expected_state) const {
    std::shared_lock lock(mutex_);
    return state_ == expected_state;
}

void ClientStateManager::SendStateToSignalingServer() const {
    if (signaling_client_) {
        signaling_client_->SendState(StateToString(state_));
    } else {
        RTC_SENDER_LOG_ERROR("Signaling server is not set, cannot send state change");
    }
}

void ClientStateManager::LogState(const State old_state, const State new_state) {
    RTC_SENDER_LOG_INFO("Changing state from {} to {}", StateToString(old_state), StateToString(new_state));
}

std::string ClientStateManager::StateToString(const State state) {
    switch (state) {
        case INITIALIZING:
            return "INITIALIZING";
        case READY_TO_CONNECT:
            return "READY_TO_CONNECT";
        case CONNECTING:
            return "CONNECTING";
        case CONNECTED:
            return "CONNECTED";
        case DISCONNECTING:
            return "DISCONNECTING";
        case FAILED:
            return "FAILED";
        case SHUTTING_DOWN:
            return "SHUTTING_DOWN";
        default:
            return "UNKNOWN_STATE";
    }
}
