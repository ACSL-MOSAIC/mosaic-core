################################################################################
# 외부 라이브러리 찾기
################################################################################
if (APPLE)
    # Apple Silicon
    if (EXISTS "/opt/homebrew")
        set(CMAKE_PREFIX_PATH "/opt/homebrew" ${CMAKE_PREFIX_PATH})
    endif ()
    # Intel
    if (EXISTS "/usr/local")
        set(CMAKE_PREFIX_PATH "/usr/local" ${CMAKE_PREFIX_PATH})
    endif ()
endif ()

find_package(PkgConfig REQUIRED)

if (POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif ()

find_package(OpenSSL REQUIRED COMPONENTS SSL Crypto)

find_package(cpprestsdk REQUIRED)

# logging 을 위한 fmt 라이브러리
find_package(fmt REQUIRED)

# opencv
find_package(OpenCV REQUIRED)
if (NOT OpenCV_FOUND)
    message(FATAL_ERROR
            "OpenCV not found!\n"
            "Please install OpenCV development packages:\n"
            "   Ubuntu/Debian: sudo apt install libopencv-dev"
    )
endif ()

pkg_check_modules(JSONCPP jsoncpp)
if (NOT JSONCPP_FOUND)
    message(FATAL_ERROR
            "JsonCPP not found via pkg-config!\n"
            "Please install JsonCPP development packages:\n"
            "   Ubuntu/Debian: sudo apt install libjsoncpp-dev"
    )
endif ()

################################################################################
# WebRTC related Settings
################################################################################
set(WEBRTC_HEADERS_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_party/webrtc-headers/src")
set(WEBRTC_INCLUDE_PATH "${WEBRTC_HEADERS_ROOT_PATH}")
set(WEBRTC_LIBRARY_PATH "${WEBRTC_HEADERS_ROOT_PATH}/out/Default/obj")

find_library(WEBRTC_LIBRARIES
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
function(create_third_party_target)

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
            ${WEBRTC_INCLUDE_PATH}
            ${WEBRTC_INCLUDE_PATH}/third_party/abseil-cpp
            ${WEBRTC_INCLUDE_PATH}/third_party/libyuv/include
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
            ${JSONCPP_LIBRARIES}
    )

endfunction()

create_third_party_target()

set(THIRD_PARTY_READY TRUE)
