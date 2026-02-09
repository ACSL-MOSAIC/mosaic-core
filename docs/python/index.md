# MOSAIC RTC Core - Python Documentation

Welcome to the Python documentation for **MOSAIC RTC Core**, a library for WebRTC-based real-time robot control and monitoring.

## Overview

MOSAIC RTC Core provides Python bindings for the C++ library, enabling:

- 🎯 **WebRTC Communication** - Peer-to-peer connections for robots
- 📡 **Data Channels** - Bidirectional data communication
- 🎥 **Media Tracks** - Video streaming capabilities
- ⚙️ **Auto Configuration** - Automatic connector setup from YAML files
- 📊 **Logging System** - Flexible logging with multiple levels

## Quick Links

- [Installation Guide](installation.md) - Get started with installation
- [Quick Start](quickstart.md) - Your first MOSAIC application
- [API Reference](api/overview.md) - Complete API documentation
- [C++ Documentation](../index.html) - C++ API reference

## Modules

The library provides the following Python modules:

### `mosaic_core.logging`
Logging system with configurable log levels and custom loggers.

### `mosaic_core.configs`
Configuration classes for WebRTC, authentication, and connectors.

### `mosaic_core.handlers`
Data channel and media track handlers for communication.

### `mosaic_core.core`
Core classes including MosaicConnector and MosaicConnectorFactory.

### `mosaic_core.auto_configurer`
Automatic configuration system for connectors from YAML files.

## Requirements

- Python 3.7 or later
- Compiled MOSAIC RTC Core library

## Example

```python
import mosaic_core

# Set up logging
mosaic_core.logging.set_log_level(mosaic_core.logging.LogLevel.INFO)

# Create configuration
config = mosaic_core.configs.ServerConfig()
config.ws_url = "wss://example.com"

# Create connector factory
factory = mosaic_core.core.MosaicConnectorFactory(config)
connector = factory.create()
```

## Getting Help

- Check the [tutorials](tutorials/logging.md) for step-by-step guides
- See [examples](examples.md) for working code samples
- Refer to the [API reference](api/overview.md) for detailed documentation

## License

This project follows the same license as the parent MOSAIC repository.