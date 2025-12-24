#!/bin/bash
set -e

THIRD_PARTY_WEBRTC_DIR="$(pwd)/webrtc"
WEBRTC_CHECKOUT_DIR="$HOME/webrtc-checkout"

# Check if WEBRTC_CHECKOUT_DIR exists and is not empty
if [ -d "$WEBRTC_CHECKOUT_DIR" ] && [ "$(ls -A "$WEBRTC_CHECKOUT_DIR")" ]; then
    echo "Warning: Directory $WEBRTC_CHECKOUT_DIR already exists and is not empty."
    echo "Contents will be completely removed to download fresh WebRTC source."
    echo -n "Do you want to continue? (y/N): "
    read -r response
    if [ "$response" != "y" ] && [ "$response" != "Y" ]; then
        echo "Operation cancelled."
        exit 0
    fi
fi

echo "Removing old WebRTC installation..."
rm -rf "$WEBRTC_CHECKOUT_DIR"
mkdir -p "$WEBRTC_CHECKOUT_DIR"

echo "Setting up depot_tools..."
if [ ! -d "/tmp/depot_tools" ]; then
    cd /tmp
    git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git || true
fi

export PATH="/tmp/depot_tools:$PATH"

echo "Downloading fresh WebRTC source..."
mkdir -p "$WEBRTC_CHECKOUT_DIR"
cd "$WEBRTC_CHECKOUT_DIR"

fetch --nohooks webrtc
cd src

echo "Checking out 6723 branch..."
git checkout branch-heads/6723

echo "Syncing dependencies..."
gclient sync

echo "Make symbolic link to third_party directory..."
ln -s "$WEBRTC_CHECKOUT_DIR" "$THIRD_PARTY_WEBRTC_DIR"
