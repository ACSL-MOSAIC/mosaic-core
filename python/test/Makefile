# Auto-detect build directory (cmake-build-debug preferred, fallback to build)
BUILD_DIR := $(firstword $(wildcard ../../build ../../cmake-build-debug))
ifeq ($(BUILD_DIR),)
$(error Build directory not found. cmake-build-debug or build directory is required.)
endif

.PHONY: test stub env_setting clean

# Create _build symlink referenced by pyproject.toml
_build:
	@ln -snf $(BUILD_DIR) _build

# Generate .pyi stubs from compiled mosaic_core.so for IDE static analysis
# Output: $(BUILD_DIR)/mosaic_core/mosaic_core.pyi
stub: _build
	uv sync --all-groups
	uv run pybind11-stubgen mosaic_core -o $(BUILD_DIR)
#	uv run pybind11-stubgen mosaic_core.handlers -o $(BUILD_DIR)/mosaic_core
#	uv run pybind11-stubgen mosaic_core.configs -o $(BUILD_DIR)/mosaic_core
#	uv run pybind11-stubgen mosaic_core.core -o $(BUILD_DIR)/mosaic_core
#	uv run pybind11-stubgen mosaic_core.auto_configurer -o $(BUILD_DIR)/mosaic_core

env_setting: clean stub

test: env_setting
	uv run python -m unittest discover -s tests

clean:
	rm -f _build
	rm -rf .venv uv.lock