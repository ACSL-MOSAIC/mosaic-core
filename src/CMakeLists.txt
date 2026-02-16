################################################################################
# WebRTC Sender Library
################################################################################
add_library(logging STATIC
        logger/log_service.cpp
        logger/webrtc_log.cpp
)
target_compile_options(logging PRIVATE -fPIC)
target_link_libraries(logging PRIVATE
        third_party_lib
)
target_include_directories(logging PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
)

add_library(signaling_server STATIC
        signaling/websocket_client.cpp
        signaling/signaling_client.cpp
)
target_compile_options(signaling_server PRIVATE -fPIC)
target_link_libraries(signaling_server PRIVATE
        logging
        cpprestsdk::cpprest
        third_party_lib
)
target_include_directories(signaling_server PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
        ${CPPREST_INCLUDE_DIRS}
)

add_library(handlers STATIC
        handlers/data_channel/data_channel_handler.cpp
        handlers/data_channel/data_channel_sendable.cpp
        handlers/data_channel/data_channel_receivable.cpp
        handlers/media_track/media_track_handler.cpp
        handlers/media_track/recordable.cpp
        handlers/media_track/video_recorder.cpp
)
target_compile_options(handlers PRIVATE -fPIC)
target_link_libraries(handlers PRIVATE
        third_party_lib
        logging
)
target_include_directories(handlers PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
)

add_library(security STATIC
        security/authenticator_resolver.cpp
        security/mosaic_authenticator.cpp
        security/impl/no_auth_authenticator.cpp
        security/impl/simple_token_authenticator.cpp
)
target_compile_options(security PRIVATE -fPIC)
target_link_libraries(security PUBLIC
        third_party_lib
        logging
)
target_include_directories(security PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
)

add_library(mosaic-rtc-core STATIC
        core/mosaic_connector.cpp
        core/connector_state_manager.cpp
        core/peer_connection_manager.cpp
        core/peer_connection_observer.cpp
)
target_compile_options(mosaic-rtc-core PRIVATE -fPIC)
target_link_libraries(mosaic-rtc-core PUBLIC
        third_party_lib
        signaling_server
        logging
        handlers
        security
)

target_include_directories(mosaic-rtc-core PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
        #        ${JSONCPP_INCLUDE_DIRS}
)

add_library(mosaic-auto-configurer STATIC
        auto_configurer/auto_configurer.cpp
        auto_configurer/connector_resolver.cpp
        config_reader/config_reader_resolver.cpp
        config_reader/yaml_config_reader.cpp
        auto_configurer/impl/opencv_camera_mt_handler.cpp
)
target_compile_options(mosaic-auto-configurer PRIVATE -fPIC)

target_include_directories(mosaic-auto-configurer PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(mosaic-auto-configurer PUBLIC
        third_party_lib
        mosaic-rtc-core
)

add_library(mosaic-core INTERFACE)
target_link_libraries(mosaic-core INTERFACE
        mosaic-rtc-core
        mosaic-auto-configurer
)