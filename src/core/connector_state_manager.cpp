//
// Created by yhkim on 25. 6. 26.
//

#include <mutex>

#include <mosaic/core/connector_state_manager.h>
#include <mosaic/logger/log.h>

using namespace mosaic::core;

ConnectorStateManager::ConnectorStateManager(const std::shared_ptr<core_signaling::ISignalingClient>& signaling_client)
    : state_(INITIALIZING), signaling_client_(signaling_client) {}

void ConnectorStateManager::SetState(const State new_state) {
    std::unique_lock lock(mutex_);
    LogState(state_, new_state);
    state_ = new_state;
    SendStateToSignalingServer();
}

ConnectorStateManager::State ConnectorStateManager::GetState() const {
    std::shared_lock lock(mutex_);
    return state_;
}

std::string ConnectorStateManager::GetStateString() const {
    std::shared_lock lock(mutex_);
    return StateToString(state_);
}

bool ConnectorStateManager::IsState(const State expected_state) const {
    std::shared_lock lock(mutex_);
    return state_ == expected_state;
}

void ConnectorStateManager::SendStateToSignalingServer() const {
    if (signaling_client_) {
        signaling_client_->SendState(StateToString(state_));
    } else {
        MOSAIC_LOG_ERROR("Signaling server is not set, cannot send state change");
    }
}

void ConnectorStateManager::LogState(const State old_state, const State new_state) {
    MOSAIC_LOG_INFO("Changing state from {} to {}", StateToString(old_state), StateToString(new_state));
}

std::string ConnectorStateManager::StateToString(const State state) {
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
