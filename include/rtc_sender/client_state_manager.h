//
// Created by yhkim on 25. 6. 26.
//

#ifndef CLIENT_STATE_MANAGER_H
#define CLIENT_STATE_MANAGER_H

#include <memory>
#include <shared_mutex>
#include <string>

namespace rtc_sender {
  // Forward declarations for avoid circular dependencies
  class SignalingServer;
} // namespace rtc_sender

namespace rtc_sender {
  class ClientStateManager final {
  public:
    enum State { INITIALIZING, READY_TO_CONNECT, CONNECTING, CONNECTED, DISCONNECTING, FAILED, SHUTTING_DOWN };

    explicit ClientStateManager(const std::shared_ptr<SignalingServer> &signaling_server);

    void SetState(State new_state);

    State GetState() const;

    std::string GetStateString() const;

    bool IsState(State expected_state) const;

  private:
    State state_;
    std::shared_ptr<SignalingServer> signaling_server_;
    mutable std::shared_mutex mutex_;

    void SendStateToSignalingServer() const;

    static void LogState(State old_state, State new_state);

    static std::string StateToString(State state);
  };
} // namespace rtc_sender

#endif  // CLIENT_STATE_MANAGER_H
