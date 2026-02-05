# Auto-detect build directory (cmake-build-debug preferred, fallback to build)
BUILD_DIR := $(firstword $(wildcard ../../cmake-build-debug ../../build))
ifeq ($(BUILD_DIR),)
$(error Build directory not found. cmake-build-debug or build directory is required.)
endif

.PHONY: sync test clean

# Create _build symlink referenced by pyproject.toml
_build:
	@ln -snf $(BUILD_DIR) _build

sync: _build
	uv sync

test: sync
	uv run python -m unittest discover -s tests

clean:
	rm -f _build
	rm -rf .venv uv.lock