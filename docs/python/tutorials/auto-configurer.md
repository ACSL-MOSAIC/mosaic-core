# Auto Configurer Tutorial

Automatically configure connectors from YAML files.

## Basic Usage

```python
import mosaic_core

# Create auto configurer
auto_configurer = mosaic_core.auto_configurer.AutoConfigurer()

# Load and configure from YAML
auto_configurer.auto_configure("config.yaml")

# Get the configured connector
connector = auto_configurer.get_mosaic_connector()
```

## Configuration File Format

Example `config.yaml`:

```yaml
server:
  ws_url: "wss://signaling.example.com"
  auth_config:
    type: "token"
    robot_id: "robot-001"
    params:
      token: "your-secret-token"
  webrtc_config:
    ice_servers:
      - urls:
          - "stun:stun.l.google.com:19302"

connectors:
  - type: "echo"
    label: "echo_channel"
    params:
      key: "value"
```

## Custom Connectors

### Register Custom Connector

```python
# Define custom connector
class MyConnector(mosaic_core.auto_configurer.IConfigurableConnector):
    def get_connector_type(self) -> str:
        return "my_custom_type"

    def validate_config(self):
        # Validation logic
        pass

    def configure(self):
        # Configuration logic
        pass

# Define factory
class MyConnectorFactory(mosaic_core.auto_configurer.IConfigurableConnectorFactory):
    def get_connector_type(self) -> str:
        return "my_custom_type"

    def create_connector(self):
        return MyConnector()

# Register factory
factory = MyConnectorFactory()
mosaic_core.auto_configurer.register_configurable_connector(factory)

# Now you can use it in YAML
auto_configurer.auto_configure("config_with_custom.yaml")
```

## See Also

- [Configuration Tutorial](configuration.md)
- [API Reference: auto_configurer](../api/auto-configurer.md)
