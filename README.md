# RTC Sender C++ Library

A C++ library for WebRTC-based real-time robot control and monitoring. This library provides a framework for
bidirectional communication between robots and control interfaces using WebRTC technology.

## Overview

The `rtc_sender` library is the core component that enables WebRTC communication for robot platforms. It provides:

- **WebRTC Client Management**: Automated peer connection setup and management
- **Data Channel Handlers**: Bidirectional data communication for control commands and telemetry
- **Media Track Handlers**: Video streaming capabilities for camera feeds and other media sources
- **Signaling Server**: WebSocket-based signaling for WebRTC connection establishment

## Library Structure

```
BA-GCS-rtc-sender/
├── include/rtc_sender/              # Public API headers
│   ├── handlers/
│   │   ├── data_channel/            # Data channel communication
│   │   └── media_track/             # Media streaming
│   ├── logger/                      # Logging utilities
│   ├── observers/                   # WebRTC event observers
│   └── websocket/                   # WebSocket client
├── src/                             # Implementation files
├── cmake/                           # CMake configuration for third party dependencies
├── third_party/                     # WebRTC setting & build scripts
└── test/                            # Unit tests (currently websocket client and logging related tests only)
```

## Installation

For detailed WebRTC setup instructions, see [docs/Installation.md](../docs/Installation.md).

### Core Dependencies

- **C++17** or later
- **[WebRTC](https://webrtc.github.io/webrtc-org/native-code/native-apis/)** - Google WebRTC library for peer-to-peer
  communication
- **[OpenSSL](https://www.openssl.org/)** - For secure WebSocket connections
- **[OpenCV](https://opencv.org/)** - Image processing and conversion
- **[JsonCpp](https://github.com/open-source-parsers/jsoncpp)** - JSON data parsing
- **[cpprestsdk](https://github.com/Microsoft/cpprestsdk)** - HTTP/WebSocket client library
- **[fmt](https://fmt.dev/)** - Modern C++ formatting library

## Integration Guide

The `rtc_sender` library uses the **PIMPL (Pointer to Implementation)** pattern to hide implementation details and
minimize dependencies for client code. This means you only need to link against `rtc_sender` - all underlying
dependencies (WebRTC, cpprestsdk, OpenSSL, etc.) are automatically handled internally.

### 1. Add Library to Your Project

Add the library as a subdirectory in your `CMakeLists.txt`:

```cmake
# Set the path to rtc-sender-cpp-lib
set(RTC_SENDER_PATH "/path/to/ai-gcs-robot/rtc-sender-cpp-lib")

# Add the library subdirectory
add_subdirectory(${RTC_SENDER_PATH} ${CMAKE_BINARY_DIR}/cmake-build-debug)
```

### 2. Link Against the Library

For your executable or library that uses rtc_sender:

```cmake
# Create your executable
add_executable(your_robot_app src/main.cpp)

# Link against rtc_sender library only
target_link_libraries(your_robot_app
        rtc_sender              # All dependencies are handled internally
)
```

**Note**: Thanks to the PIMPL pattern implementation, you don't need to explicitly link against WebRTC, cpprestsdk,
OpenSSL, or other internal dependencies. The `rtc_sender` library manages all these dependencies internally.

### 3. Include Headers

rtc_sender.h is the main header file that includes all necessary components:

```cpp
#include "rtc_sender.h"
```

## Basic Usage Example

### 1. Create WebRTC Client

```cpp
#include "rtc_sender.h"

// Configure client
rtc_sender::RobotWebRTCClientFactory factory(
    "robot_id_123",           // robot ID
    "user_id_456",            // user ID  
    "ws://localhost:8080",    // WebSocket signaling server
    "turn:server.com:3478",   // TURN server URL
    "username",               // TURN username
    "password"                // TURN password
);

// Create client instance
auto webrtc_client = factory.Create();
```

### 2. Register Custom Handlers and Start

```cpp
// Create handlers
auto control_channel = std::make_shared<RobotControlChannel>();
auto camera_track = std::make_shared<CameraMediaTrack>();

// Register with client
webrtc_client->AddDataChannelHandler(control_channel);
webrtc_client->AddMediaTrackHandler(camera_track);

// Start the client
webrtc_client->StartSignalingServer();

// Cleanup
webrtc_client->ShuttingDown();
```

For comprehensive implementation guidance and examples:

- [Implementation Guide](../docs/Implementation.md)

## ROS Integration

For ROS1/ROS2 integration examples, see the [examples](../examples/README.md) directory:

- **ROS1 Example**: [husky_ros1](../examples/husky_ros1/README.md)
- **ROS2 Example**: [husky_ros2](../examples/husky_ros2/README.md)

## Library Components

### Core Classes

- **`RobotWebRTCClient`**: Main WebRTC client interface
- **`RobotWebRTCClientFactory`**: Factory for creating client instances

### Handlers

- **`DataChannelReceivable<T>`**: Template for receiving typed data
- **`DataChannelSendable`**: Base class for sending data
- **`MediaTrackHandler`**: Base class for media streaming

### Utilities

- **`LogService`**: Centralized logging system
- **`WebSocketClient`**: Wrapper for WebSocket communication
- **`ClientStateManager`**: Connection state management for RobotWebRTCClient

## License

This project follows the same license as the parent BA-GCS-Robot repository.

## Install

```shell
mkdir build && cd build
cmake ..
make
sudo make install
```