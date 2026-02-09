# Handlers Tutorial

Learn how to create and use data channel and media track handlers.

## Data Channel Handlers

### String Receiver

```python
import mosaic_core

class MyStringHandler(mosaic_core.handlers.data_channel.DataChannelStringReceivable):
    def __init__(self, label):
        super().__init__(label)

    def handle_data(self, data: str):
        print(f"Received string: {data}")

# Create and register
handler = MyStringHandler("my_channel")
connector.add_data_channel_handler(handler)
```

### JSON Receiver

```python
class MyJsonHandler(mosaic_core.handlers.data_channel.DataChannelJsonReceivable):
    def __init__(self, label):
        super().__init__(label)

    def handle_data(self, data: dict):
        print(f"Received JSON: {data}")
        # Access JSON fields
        if "command" in data:
            self.process_command(data["command"])

    def process_command(self, command):
        print(f"Processing: {command}")

handler = MyJsonHandler("control")
connector.add_data_channel_handler(handler)
```

### Binary Receiver

```python
class MyByteHandler(mosaic_core.handlers.data_channel.DataChannelByteReceivable):
    def __init__(self, label):
        super().__init__(label)

    def handle_data(self, data: bytes):
        print(f"Received {len(data)} bytes")

handler = MyByteHandler("binary_channel")
connector.add_data_channel_handler(handler)
```

### Data Sender

```python
class MySender(mosaic_core.handlers.data_channel.DataChannelSendable):
    def __init__(self, label):
        super().__init__(label)

    def send_message(self, message: str):
        if self.sendable():
            self.send_string(message)
        else:
            print("Channel not ready")

sender = MySender("output")
connector.add_data_channel_handler(sender)

# Send data
sender.send_message("Hello!")
```

## Media Track Handlers

### Video Handler

```python
import numpy as np

class MyVideoHandler(mosaic_core.handlers.media_track.AMediaTrackHandler):
    def __init__(self, track_name):
        super().__init__(track_name, recordable=False)

    def start(self):
        self.set_running(True)
        # Start video streaming loop
        while self.is_running() and not self.get_stop_flag():
            frame = self.capture_frame()
            self.send_frame(frame)

    def stop(self):
        self.set_running(False)

    def capture_frame(self) -> np.ndarray:
        # Your frame capture logic
        return np.zeros((480, 640, 3), dtype=np.uint8)

video = MyVideoHandler("camera")
connector.add_media_track_handler(video)
```

### Recording

```python
class RecordableVideoHandler(mosaic_core.handlers.media_track.AMediaTrackHandler):
    def __init__(self, track_name):
        super().__init__(track_name, recordable=True)
        
        # Set recording parameters
        self.set_video_parameters(fps=30, width=640, height=480)
        self.set_record_file_path("output.mp4")

    def start(self):
        # Start recording
        self.start_recording()
        
        self.set_running(True)
        while self.is_running() and not self.get_stop_flag():
            frame = self.capture_frame()
            
            # Send frame
            self.send_frame(frame)
            
            # Record frame
            self.record_frame(frame)

    def stop(self):
        self.set_running(False)
        self.stop_recording()
```

## See Also

- [API Reference: handlers](../api/handlers.md)
- [Examples](../examples.md)
