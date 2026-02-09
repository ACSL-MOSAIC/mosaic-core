# Quick Start

This guide will help you create your first MOSAIC RTC Core application in Python.

## Basic Example

Here's a minimal example to get started:

```python
import mosaic_core

# Set up logging
mosaic_core.logging.set_log_level(mosaic_core.logging.LogLevel.INFO)

# Create server configuration
server_config = mosaic_core.configs.ServerConfig()
server_config.ws_url = "wss://your-signaling-server.com"

# Set up authentication
server_config.auth_config.type = "token"
server_config.auth_config.robot_id = "robot-001"
server_config.auth_config.params = {"token": "your-token"}

# Create WebRTC configuration
ice_server = mosaic_core.configs.IceServerConfig()
ice_server.urls = ["stun:stun.l.google.com:19302"]
server_config.webrtc_config.ice_servers.append(ice_server)

# Create MosaicConnector
factory = mosaic_core.core.MosaicConnectorFactory(server_config)
connector = factory.create()

# Initialize WebRTC
connector.initialize_webrtc()

print("MOSAIC RTC Core is ready!")
```

## Using Data Channels

### Creating a Data Channel Handler

```python
class MyDataChannelHandler(mosaic_core.handlers.data_channel.DataChannelStringReceivable):
    def __init__(self, label):
        super().__init__(label)

    def handle_data(self, data):
        print(f"Received: {data}")

# Create and add handler
handler = MyDataChannelHandler("my_channel")
connector.add_data_channel_handler(handler)
```

## Using Media Tracks

### Creating a Media Track Handler

```python
import numpy as np

class MyVideoHandler(mosaic_core.handlers.media_track.AMediaTrackHandler):
    def __init__(self, track_name):
        super().__init__(track_name, recordable=False)

    def start(self):
        self.set_running(True)
        print("Video handler started")

    def stop(self):
        self.set_running(False)
        print("Video handler stopped")

    def send_frame(self, frame: np.ndarray):
        # Send video frame
        super().send_frame(frame)

# Create and add handler
video_handler = MyVideoHandler("camera")
connector.add_media_track_handler(video_handler)
```

## Using Auto Configurer

Load configuration from YAML file:

```python
# Create auto configurer
auto_configurer = mosaic_core.auto_configurer.AutoConfigurer()

# Load and configure from YAML
auto_configurer.auto_configure("config.yaml")

# Get the configured connector
connector = auto_configurer.get_mosaic_connector()
```

## Complete Example

```python
import mosaic_core
import time

# Set up logging
mosaic_core.logging.set_log_level(mosaic_core.logging.LogLevel.DEBUG)

# Use auto configurer for easy setup
auto_configurer = mosaic_core.auto_configurer.AutoConfigurer()
auto_configurer.auto_configure("examples/config.yaml")

connector = auto_configurer.get_mosaic_connector()

# Application runs...
try:
    print("Press Ctrl+C to exit")
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("\nShutting down...")
    connector.shutting_down()
```

## Next Steps

- [Logging Tutorial](tutorials/logging.md) - Learn about logging configuration
- [Configuration Tutorial](tutorials/configuration.md) - Understand configurations
- [Handler Tutorial](tutorials/handlers.md) - Create custom handlers
- [API Reference](api/overview.md) - Explore the full API