# Installation

## Prerequisites

- Python 3.7 or later
- C++ compiler with C++17 support
- CMake 3.15 or later
- Git

## From Source

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/mosaic-rtc-core.git
cd mosaic-rtc-core
```

### 2. Build the Library

```bash
mkdir build && cd build
cmake ..
make
```

### 3. Install Python Bindings

```bash
cd ../python
pip install -e .
```

## Verify Installation

```python
import mosaic_core
print(mosaic_core.__version__)
print(mosaic_core.__all__)
```

You should see:
```
1.0.0
['logging', 'configs', 'handlers', 'core', 'auto_configurer']
```

## Development Installation

For development with editable installation:

```bash
cd python
pip install -e . --verbose
```

## Troubleshooting

### Import Error

If you get `ImportError: No module named '_mosaic_core'`:

1. Make sure the C++ library is built:
   ```bash
   cd build
   make
   ```

2. Check that the Python extension is in the correct location:
   ```bash
   ls python/mosaic_core/_mosaic_core*.so
   ```

### Build Errors

If CMake cannot find dependencies:

```bash
# Install required dependencies (Ubuntu/Debian)
sudo apt-get install libssl-dev libopencv-dev libjsoncpp-dev

# Or use vcpkg (cross-platform)
vcpkg install openssl opencv jsoncpp
```

## Next Steps

- [Quick Start Guide](quickstart.md) - Create your first application
- [Logging Tutorial](tutorials/logging.md) - Learn about logging