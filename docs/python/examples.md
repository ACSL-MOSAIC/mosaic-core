# Examples

Complete working examples for MOSAIC RTC Core.

## Basic Echo Server

Simple echo server that receives and sends back data:

```python
import mosaic_core
import time

# Custom data channel handler
class EchoHandler(mosaic_core.handlers.data_channel.DataChannelStringReceivable):
    def __init__(self, label):
        super().__init__(label)
        self.sendable = mosaic_core.handlers.data_channel.DataChannelSendable(label)

    def handle_data(self, data: str):
        print(f"Received: {data}")
        # Echo back
        self.sendable.send_string(f"Echo: {data}")

# Set up logging
mosaic_core.logging.set_log_level(mosaic_core.logging.LogLevel.INFO)

# Configure server
config = mosaic_core.configs.ServerConfig()
config.ws_url = "wss://your-server.com"
config.auth_config.type = "token"
config.auth_config.robot_id = "echo-bot"

# Create connector
factory = mosaic_core.core.MosaicConnectorFactory(config)
connector = factory.create()
connector.initialize_webrtc()

# Add echo handler
echo = EchoHandler("echo")
connector.add_data_channel_handler(echo)

# Run
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    connector.shutting_down()
```

## Video Streaming

Stream video frames:

```python
import mosaic_core
import numpy as np
import cv2

class VideoStreamer(mosaic_core.handlers.media_track.AMediaTrackHandler):
    def __init__(self, track_name, video_source=0):
        super().__init__(track_name, recordable=False)
        self.cap = cv2.VideoCapture(video_source)

    def start(self):
        self.set_running(True)
        while self.is_running() and not self.get_stop_flag():
            ret, frame = self.cap.read()
            if ret:
                # Send frame
                self.send_frame(frame)
            else:
                break

    def stop(self):
        self.set_running(False)
        self.cap.release()

# Set up
mosaic_core.logging.set_log_level(mosaic_core.logging.LogLevel.INFO)

# Use auto configurer
auto_configurer = mosaic_core.auto_configurer.AutoConfigurer()
auto_configurer.auto_configure("config.yaml")
connector = auto_configurer.get_mosaic_connector()

# Add video handler
video = VideoStreamer("camera")
connector.add_media_track_handler(video)

# Run
try:
    import time
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    connector.shutting_down()
```

## Custom Logger

File logging example:

```python
import mosaic_core
from datetime import datetime

class FileLogger(mosaic_core.logging.ILogger):
    def __init__(self, filename="app.log"):
        super().__init__()
        self.file = open(filename, 'a')

    def log(self, message: str, log_level: mosaic_core.logging.LogLevel):
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        self.file.write(f"[{timestamp}] [{log_level.name}] {message}\n")
        self.file.flush()

    def __del__(self):
        if hasattr(self, 'file'):
            self.file.close()

# Use custom logger
logger = FileLogger("mosaic.log")
mosaic_core.logging.set_logger(logger)
mosaic_core.logging.set_log_level(mosaic_core.logging.LogLevel.DEBUG)
```

## Configuration from YAML

Example `config.yaml`:

```yaml
server:
  ws_url: "wss://example.com"
  auth_config:
    type: "token"
    robot_id: "robot-001"
    params:
      token: "your-secret-token"
  webrtc_config:
    ice_servers:
      - urls:
          - "stun:stun.l.google.com:19302"
      - urls:
          - "turn:turn.example.com:3478"
        username: "user"
        credential: "pass"

connectors:
  - type: "echo"
    label: "echo_channel"
    params:
      key: "value"
```

Using the configuration:

```python
import mosaic_core

# Load configuration
auto_configurer = mosaic_core.auto_configurer.AutoConfigurer()
auto_configurer.auto_configure("config.yaml")

# Get connector
connector = auto_configurer.get_mosaic_connector()

# Access configuration
mosaic_config = auto_configurer.mosaic_config
print(f"Server URL: {mosaic_config.server.ws_url}")
print(f"Connectors: {len(mosaic_config.connectors)}")
```

## See Also

- [Quick Start](quickstart.md)
- [Tutorials](tutorials/logging.md)
- [API Reference](api/overview.md)