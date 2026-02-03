#!/bin/bash
set -e

WEBRTC_CHECKOUT_DIR="$HOME/webrtc-checkout"

cd "$WEBRTC_CHECKOUT_DIR/src"

echo "Installing build dependencies..."
./build/install-build-deps.sh
./build/install-build-deps.sh --no-chromeos-fonts --no-prompt --arm
./build/linux/sysroot_scripts/install-sysroot.py --arch=arm64

echo "System Information:"
echo "  Architecture: $(uname -m)"
echo "  OS: $(uname -s)"
echo "  GCC: $(gcc --version | head -1)"

echo "Configuring build..."
mkdir -p out/Default

echo "Generating build files..."
gn gen out/Default --args='is_debug=false target_os="linux" target_cpu="arm64" is_component_build=false rtc_include_tests=false use_custom_libcxx=false treat_warnings_as_errors=false use_ozone=true rtc_use_x11=false use_rtti=true rtc_build_examples=false rtc_build_json=false rtc_enable_protobuf=false'

echo "Starting build..."
ninja -C out/Default 2>&1 | tee build.log

if [ $? -ne 0 ]; then
    echo "Build failed. Check build.log for details."
    exit 1
fi

echo ""
echo "Verifying build results..."

if [ -f "out/Default/obj/libwebrtc.a" ]; then
    echo "libwebrtc.a created successfully"

    # 파일 정보
    echo "File info:"
    file out/Default/obj/libwebrtc.a

    # 아키텍처 확인
    echo "Architecture info:"
    objdump -f out/Default/obj/libwebrtc.a | head -10

    # 파일 크기 확인
    echo "File size:"
    ls -lh out/Default/obj/libwebrtc.a

    echo ""
    echo "WebRTC build completed successfully!"
    echo "Library location: $WEBRTC_CHECKOUT_DIR/src/out/Default/obj/libwebrtc.a"

else
    echo "Build failed - libwebrtc.a not found"
    echo "Check build.log for details"
    exit 1
fi
