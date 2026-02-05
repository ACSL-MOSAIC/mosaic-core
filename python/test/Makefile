# Auto-detect build directory (cmake-build-debug preferred, fallback to build)
BUILD_DIR := $(firstword $(wildcard ../../cmake-build-debug ../../build))
ifeq ($(BUILD_DIR),)
$(error Build directory not found. cmake-build-debug or build directory is required.)
endif

.PHONY: test stub env_setting clean _build

# Copy build output referenced by pyproject.toml
_build:
	@rm -rf _build
	@cp -r $(BUILD_DIR) _build

# Generate .pyi stubs from compiled _mosaic_core.so for IDE static analysis
# Output: $(BUILD_DIR)/mosaic_core/_mosaic_core.pyi
stub: _build
	uv sync --all-groups
	uv run pybind11-stubgen mosaic_core._mosaic_core -o $(BUILD_DIR)

env_setting: clean stub

test: env_setting
	uv run python -m unittest discover -s tests

clean:
	rm -rf _build .venv uv.lock