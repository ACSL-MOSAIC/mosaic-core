//
// Created by yhkim on 25. 6. 26.
//

#ifndef MOSAIC_CORE_CONNECTOR_STATE_MANAGER_HPP
#define MOSAIC_CORE_CONNECTOR_STATE_MANAGER_HPP

#include <atomic>
#include <condition_variable>
#include <memory>
#include <shared_mutex>
#include <string>
#include <thread>

#include "../signaling/i_signaling_client.hpp"

namespace mosaic::core {
class ConnectorStateManager final {
  public:
    enum State {
        INITIALIZING,
        READY_TO_CONNECT,
        RTC_CONNECTING,
        RTC_CONNECTED,
        RTC_DISCONNECTING,
        RTC_FAILED,
        DISCONNECTED,
    };

    explicit ConnectorStateManager(const std::shared_ptr<core_signaling::ISignalingClient>& signaling_client);

    ~ConnectorStateManager();

    void SetState(State new_state);

    State GetState() const;

    std::string GetStateString() const;

    bool IsState(State expected_state) const;

    static std::string StateToString(State state);
    static int StateToInt(State state);

  private:
    State state_;
    std::shared_ptr<core_signaling::ISignalingClient> signaling_client_;
    mutable std::shared_mutex mutex_;
    std::atomic<bool> stop_flag_{false};
    std::condition_variable_any cv_;
    std::thread state_broadcast_thread_;

    void SendState() const;
    void StateBroadcastWorker();

    static void LogState(State old_state, State new_state);
};
}  // namespace mosaic::core

#endif  // MOSAIC_CORE_CONNECTOR_STATE_MANAGER_HPP
