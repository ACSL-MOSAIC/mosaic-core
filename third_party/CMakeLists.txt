################################################################################
# WebRTC related Settings
################################################################################
set(WEBRTC_HEADERS_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/webrtc/src")
set(WEBRTC_INCLUDE_PATH "${WEBRTC_HEADERS_ROOT_PATH}")
set(WEBRTC_LIBRARY_PATH "${WEBRTC_HEADERS_ROOT_PATH}/out/Default/obj")

find_library(WEBRTC_LIBRARIES REQUIRED
        NAMES webrtc libwebrtc
        HINTS ${WEBRTC_LIBRARY_PATH}
        PATH_SUFFIXES lib
)

if (WIN32)
    set(WEBRTC_SYSTEM_LIBS
            winmm dmoguids wmcodecdspuuid amstrmid msdmo
            Secur32 iphlpapi ws2_32 crypt32
    )
elseif (UNIX AND NOT APPLE)
    set(WEBRTC_SYSTEM_LIBS
            dl pthread
    )
elseif (APPLE)
    set(WEBRTC_SYSTEM_LIBS
            "-framework Foundation"
            "-framework CoreAudio"
            "-framework AudioToolbox"
            "-framework AVFoundation"
            "-framework CoreGraphics"
            "-framework CoreMedia"
            "-framework CoreVideo"
    )
endif ()

################################################################################
# Generate Third Party Library Target
################################################################################
add_library(third_party_lib INTERFACE)

if (APPLE)
    target_compile_definitions(third_party_lib INTERFACE
            WEBRTC_MAC
            WEBRTC_POSIX
            NDEBUG
    )
elseif (UNIX)
    target_compile_definitions(third_party_lib INTERFACE
            WEBRTC_POSIX
            WEBRTC_LINUX
            NDEBUG
    )
elseif (WIN32)
    target_compile_definitions(third_party_lib INTERFACE
            WEBRTC_WIN
            NDEBUG
    )
endif ()

target_include_directories(third_party_lib SYSTEM INTERFACE
        $<BUILD_INTERFACE:${WEBRTC_INCLUDE_PATH}>
        $<BUILD_INTERFACE:${WEBRTC_INCLUDE_PATH}/third_party/abseil-cpp>
        $<BUILD_INTERFACE:${WEBRTC_INCLUDE_PATH}/third_party/libyuv/include>
        ${OpenCV_INCLUDE_DIRS}
        ${FMT_INCLUDE_DIRS}
        ${JSONCPP_INCLUDE_DIRS}
)

# Link WebRTC .a file
if (WEBRTC_LIBRARIES)
    target_link_libraries(third_party_lib INTERFACE
            ${WEBRTC_LIBRARIES})
endif ()

# Link system libraries required by WebRTC
if (WEBRTC_SYSTEM_LIBS)
    target_link_libraries(third_party_lib INTERFACE
            ${WEBRTC_SYSTEM_LIBS})
endif ()

target_link_libraries(third_party_lib INTERFACE
        fmt::fmt
        ${OpenCV_LIBS}
        JsonCpp::JsonCpp
        yaml-cpp::yaml-cpp
)
