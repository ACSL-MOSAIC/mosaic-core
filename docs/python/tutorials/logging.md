# Logging Tutorial

The logging module provides a flexible logging system with configurable log levels.

## Basic Usage

### Setting Log Level

```python
import mosaic_core

# Set global log level
mosaic_core.logging.set_log_level(mosaic_core.logging.LogLevel.DEBUG)

# Check current level
current_level = mosaic_core.logging.get_log_level()
print(f"Current log level: {current_level}")
```

## Log Levels

Available log levels (from most to least verbose):

| Level | Description |
|-------|-------------|
| `DEBUG` | Detailed information for debugging |
| `INFO` | General informational messages |
| `WARNING` | Warning messages |
| `ERROR` | Error messages |

## Custom Logger

You can create a custom logger by inheriting from `ILogger`:

```python
import mosaic_core

class MyCustomLogger(mosaic_core.logging.ILogger):
    def __init__(self):
        super().__init__()
        self.messages = []

    def log(self, message: str, log_level: mosaic_core.logging.LogLevel):
        # Custom logging logic
        self.messages.append((message, log_level))
        print(f"[{log_level.name}] {message}")

# Use custom logger
custom_logger = MyCustomLogger()
mosaic_core.logging.set_logger(custom_logger)

# Now all logs go through your custom logger
mosaic_core.logging.set_log_level(mosaic_core.logging.LogLevel.INFO)
```

## Filtering Logs

Check if specific log levels are enabled:

```python
if mosaic_core.logging.ILogger.is_debug_enabled():
    # Do expensive debug operations
    debug_info = collect_debug_info()
    logger.log_debug(debug_info)
```

## WebRTC Logging

Configure WebRTC internal logging:

```python
# Set WebRTC log level
mosaic_core.logging.set_webrtc_log_level(
    mosaic_core.logging.WebRTCLogLevel.LS_INFO
)

# Enable timestamps
mosaic_core.logging.set_webrtc_log_timestamps(True)

# Show thread IDs
mosaic_core.logging.set_webrtc_log_threads(True)
```

## Complete Example

```python
import mosaic_core

class FileLogger(mosaic_core.logging.ILogger):
    def __init__(self, filename):
        super().__init__()
        self.file = open(filename, 'a')

    def log(self, message: str, log_level: mosaic_core.logging.LogLevel):
        import datetime
        timestamp = datetime.datetime.now().isoformat()
        self.file.write(f"[{timestamp}] [{log_level.name}] {message}\n")
        self.file.flush()

    def __del__(self):
        self.file.close()

# Set up file logger
file_logger = FileLogger("app.log")
mosaic_core.logging.set_logger(file_logger)
mosaic_core.logging.set_log_level(mosaic_core.logging.LogLevel.DEBUG)

# Your application code
print("Logging is configured!")
```

## See Also

- [API Reference: logging](../api/logging.md)
- [Quick Start](../quickstart.md)