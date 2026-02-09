# API Reference Overview

Complete API reference for MOSAIC RTC Core Python bindings.

## Modules

### [mosaic_core.logging](logging.md)
Logging system with configurable log levels and custom logger support.

**Key Classes:**
- `ILogger` - Base logger interface
- `LogLevel` - Log level enumeration
- `WebRTCLogLevel` - WebRTC log level enumeration

**Key Functions:**
- `set_log_level()` - Set global log level
- `get_log_level()` - Get current log level
- `set_logger()` - Set custom logger
- `get_logger()` - Get current logger

### [mosaic_core.configs](configs.md)
Configuration classes for server, authentication, and WebRTC settings.

**Key Classes:**
- `ServerConfig` - Server configuration
- `AuthConfig` - Authentication configuration
- `WebRtcConfig` - WebRTC configuration
- `IceServerConfig` - ICE server configuration
- `ConnectorConfig` - Connector configuration
- `MosaicConfig` - Complete mosaic configuration

### [mosaic_core.handlers](handlers.md)
Data channel and media track handlers.

**Submodules:**
- `handlers.data_channel` - Data channel handlers
- `handlers.media_track` - Media track handlers

**Key Classes:**
- `IDataChannelHandler` - Data channel handler interface
- `DataChannelStringReceivable` - String data receiver
- `DataChannelJsonReceivable` - JSON data receiver
- `DataChannelByteReceivable` - Binary data receiver
- `DataChannelSendable` - Data sender
- `IMediaTrackHandler` - Media track handler interface
- `AMediaTrackHandler` - Abstract media track handler
- `Recordable` - Recording functionality

### [mosaic_core.core](core.md)
Core classes for connector management.

**Key Classes:**
- `MosaicConnector` - Main connector class
- `MosaicConnectorFactory` - Factory for creating connectors

### [mosaic_core.auto_configurer](auto-configurer.md)
Automatic configuration system.

**Key Classes:**
- `AutoConfigurer` - Automatic connector configuration
- `IConfigurableConnector` - Configurable connector interface
- `IConfigurableConnectorFactory` - Connector factory interface
- `ADCHandlerConfigurer` - Data channel handler configurer
- `AMTHandlerConfigurer` - Media track handler configurer

**Key Functions:**
- `register_configurable_connector()` - Register custom connector
- `is_supported_connector_type()` - Check if type is supported
- `get_configurable_connector()` - Get connector by type

## Type Annotations

For better IDE support, you can generate type stubs:

```bash
pip install pybind11-stubgen
pybind11-stubgen mosaic_core -o stubs
```

## See Also

- [C++ Documentation](../../index.html) - C++ API reference
- [Tutorials](../tutorials/logging.md) - Step-by-step guides
- [Examples](../examples.md) - Working code samples