# mosaic_core.logging

Logging system with configurable log levels and custom logger support.

## Classes

### ILogger

Base class for custom loggers.

```python
class ILogger:
    def log(self, message: str, log_level: LogLevel) -> None:
        """Log a message with specified level."""
        pass

    def log_debug(self, message: str) -> None:
        """Log a debug message."""
        pass

    def log_info(self, message: str) -> None:
        """Log an info message."""
        pass

    def log_warning(self, message: str) -> None:
        """Log a warning message."""
        pass

    def log_error(self, message: str) -> None:
        """Log an error message."""
        pass

    @staticmethod
    def get_log_level() -> LogLevel:
        """Get current log level."""
        pass

    @staticmethod
    def is_debug_enabled() -> bool:
        """Check if debug logging is enabled."""
        pass

    @staticmethod
    def is_info_enabled() -> bool:
        """Check if info logging is enabled."""
        pass

    @staticmethod
    def is_warning_enabled() -> bool:
        """Check if warning logging is enabled."""
        pass

    @staticmethod
    def is_error_enabled() -> bool:
        """Check if error logging is enabled."""
        pass
```

**Example:**
```python
class MyLogger(mosaic_core.logging.ILogger):
    def log(self, message: str, log_level: mosaic_core.logging.LogLevel):
        print(f"[{log_level.name}] {message}")

logger = MyLogger()
mosaic_core.logging.set_logger(logger)
```

### LogLevel

Enumeration of log levels.

**Members:**
- `DEBUG` - Debug level (most verbose)
- `INFO` - Information level
- `WARNING` - Warning level
- `ERROR` - Error level (least verbose)

**Example:**
```python
mosaic_core.logging.set_log_level(mosaic_core.logging.LogLevel.INFO)
```

### WebRTCLogLevel

Enumeration of WebRTC log levels.

**Members:**
- `LS_VERBOSE` - Verbose logging
- `LS_INFO` - Info logging
- `LS_WARNING` - Warning logging
- `LS_ERROR` - Error logging
- `LS_NONE` - No logging

## Functions

### set_log_level

```python
def set_log_level(level: LogLevel) -> None:
    """Set the global logging level."""
```

**Parameters:**
- `level` (LogLevel): The log level to set

**Example:**
```python
mosaic_core.logging.set_log_level(mosaic_core.logging.LogLevel.DEBUG)
```

### get_log_level

```python
def get_log_level() -> LogLevel:
    """Get the current global logging level."""
```

**Returns:**
- LogLevel: Current log level

### set_logger

```python
def set_logger(logger: ILogger) -> None:
    """Set a custom logger."""
```

**Parameters:**
- `logger` (ILogger): Custom logger instance

### get_logger

```python
def get_logger() -> ILogger:
    """Get the current logger instance."""
```

**Returns:**
- ILogger: Current logger

### set_webrtc_log_level

```python
def set_webrtc_log_level(level: WebRTCLogLevel) -> None:
    """Set WebRTC internal log level."""
```

**Parameters:**
- `level` (WebRTCLogLevel): WebRTC log level

### set_webrtc_log_timestamps

```python
def set_webrtc_log_timestamps(enable: bool) -> None:
    """Enable or disable timestamps in WebRTC logs."""
```

**Parameters:**
- `enable` (bool): True to enable, False to disable

### set_webrtc_log_threads

```python
def set_webrtc_log_threads(enable: bool) -> None:
    """Enable or disable thread IDs in WebRTC logs."""
```

**Parameters:**
- `enable` (bool): True to enable, False to disable

## See Also

- [Logging Tutorial](../tutorials/logging.md)
- [API Overview](overview.md)