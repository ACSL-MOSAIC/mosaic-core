################################################################################
# Mosaic Core Library
################################################################################

# Automatically determine STATIC/SHARED based on BUILD_SHARED_LIBS
# ON = SHARED, OFF = STATIC
if (BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
else ()
    set(LIB_TYPE STATIC)
endif ()

# Single unified library containing ALL components
add_library(mosaic-core ${LIB_TYPE}
        # Logger
        logger/log_service.cpp
        logger/webrtc_log.cpp
        # Signaling
        signaling/websocket_client.cpp
        signaling/signaling_client.cpp
        # Handlers
        handlers/data_channel/data_channel_handler.cpp
        handlers/data_channel/data_channel_sendable.cpp
        handlers/data_channel/data_channel_receivable.cpp
        handlers/media_track/media_track_handler.cpp
        handlers/media_track/recordable.cpp
        handlers/media_track/video_recorder.cpp
        # Security
        security/authenticator_resolver.cpp
        security/mosaic_authenticator.cpp
        security/impl/no_auth_authenticator.cpp
        security/impl/simple_token_authenticator.cpp
        # Core
        core/mosaic_connector.cpp
        core/mosaic_connector_factory.cpp
        core/connector_state_manager.cpp
        core/peer_connection_manager.cpp
        core/peer_connection_observer.cpp
        # Auto Configurer
        auto_configurer/auto_configurer.cpp
        auto_configurer/connector_resolver.cpp
        config_reader/config_reader_resolver.cpp
        config_reader/yaml_config_reader.cpp
        auto_configurer/impl/opencv_camera_mt_handler.cpp
        auto_configurer/impl/connection_check_dc_handler.cpp
)

# Set version info for shared libraries
if (BUILD_SHARED_LIBS)
    set_target_properties(mosaic-core PROPERTIES
            VERSION ${PROJECT_VERSION}
            SOVERSION ${VERSION_MAJOR}
    )
else ()
    # -fPIC only needed for STATIC (SHARED has it by default)
    target_compile_options(mosaic-core PRIVATE -fPIC)
endif ()

target_link_libraries(mosaic-core PUBLIC
        third_party_lib
)

target_include_directories(mosaic-core PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
)
