# Configuration Tutorial

Learn how to configure MOSAIC RTC Core for your application.

## Basic Configuration

### Server Configuration

```python
import mosaic_core

# Create server config
config = mosaic_core.configs.ServerConfig()
config.ws_url = "wss://your-signaling-server.com"

# Get signaling URL
signaling_url = config.get_signaling_url()
print(f"Signaling URL: {signaling_url}")
```

### Authentication Configuration

```python
# Set up authentication
auth_config = mosaic_core.configs.AuthConfig()
auth_config.type = "token"
auth_config.robot_id = "robot-001"
auth_config.params = {"token": "your-secret-token"}

# Add to server config
config.auth_config = auth_config
```

### WebRTC Configuration

```python
# Create ICE server configuration
ice_server = mosaic_core.configs.IceServerConfig()
ice_server.urls = ["stun:stun.l.google.com:19302"]

# For TURN server (with authentication)
turn_server = mosaic_core.configs.IceServerConfig()
turn_server.urls = ["turn:turn.example.com:3478"]
turn_server.username = "username"
turn_server.credential = "password"

# Add to WebRTC config
webrtc_config = mosaic_core.configs.WebRtcConfig()
webrtc_config.ice_servers.append(ice_server)
webrtc_config.ice_servers.append(turn_server)

config.webrtc_config = webrtc_config
```

## Complete Configuration Example

```python
import mosaic_core

def create_config():
    # Server configuration
    config = mosaic_core.configs.ServerConfig()
    config.ws_url = "wss://signaling.example.com"
    
    # Authentication
    config.auth_config.type = "token"
    config.auth_config.robot_id = "my-robot"
    config.auth_config.params = {"token": "secret123"}
    
    # WebRTC - STUN server
    stun = mosaic_core.configs.IceServerConfig()
    stun.urls = ["stun:stun.l.google.com:19302"]
    
    # WebRTC - TURN server
    turn = mosaic_core.configs.IceServerConfig()
    turn.urls = ["turn:turn.example.com:3478"]
    turn.username = "user"
    turn.credential = "pass"
    
    config.webrtc_config.ice_servers = [stun, turn]
    
    return config

# Use the configuration
config = create_config()
factory = mosaic_core.core.MosaicConnectorFactory(config)
connector = factory.create()
```

## See Also

- [Auto Configurer Tutorial](auto-configurer.md)
- [API Reference: configs](../api/configs.md)
