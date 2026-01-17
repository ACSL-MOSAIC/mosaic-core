//
// Created by yhkim on 25. 6. 26.
//

#ifndef MOSAIC_RTC_CORE_CONNECTOR_STATE_MANAGER_H
#define MOSAIC_RTC_CORE_CONNECTOR_STATE_MANAGER_H

#include <memory>
#include <shared_mutex>
#include <string>

#include <mosaic/signaling/i_signaling_client.h>

namespace mosaic::core {
class ConnectorStateManager final {
  public:
    enum State { INITIALIZING, READY_TO_CONNECT, CONNECTING, CONNECTED, DISCONNECTING, FAILED, SHUTTING_DOWN };

    explicit ConnectorStateManager(const std::shared_ptr<core_signaling::ISignalingClient>& signaling_client);

    void SetState(State new_state);

    State GetState() const;

    std::string GetStateString() const;

    bool IsState(State expected_state) const;

  private:
    State state_;
    std::shared_ptr<core_signaling::ISignalingClient> signaling_client_;
    mutable std::shared_mutex mutex_;

    void SendState() const;

    static void LogState(State old_state, State new_state);

    static std::string StateToString(State state);
};
}  // namespace mosaic::core

#endif  // MOSAIC_RTC_CORE_CONNECTOR_STATE_MANAGER_H
