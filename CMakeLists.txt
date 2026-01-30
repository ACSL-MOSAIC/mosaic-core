cmake_minimum_required(VERSION 3.14)
project(mosaic-core VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################
# Include Third Party Libraries
################################################################################
include(cmake/ThirdPartyLibs.cmake)
if (NOT THIRD_PARTY_READY)
    message(FATAL_ERROR "Third party libraries not properly configured!")
endif ()

################################################################################
# WebRTC Sender Library
################################################################################
add_library(logging STATIC
        src/logger/log_service.cpp
        src/logger/webrtc_log.cpp
)
target_link_libraries(logging PRIVATE
        third_party_lib
)
target_include_directories(logging PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_library(signaling_server STATIC
        src/signaling/websocket_client.cpp
        src/signaling/signaling_client.cpp
)
target_link_libraries(signaling_server PRIVATE
        logging
        cpprestsdk::cpprest
        third_party_lib
)
target_include_directories(signaling_server PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${CPPREST_INCLUDE_DIRS}
)

add_library(handlers STATIC
        src/handlers/data_channel/a_data_channel_handler.cpp
        src/handlers/data_channel/data_channel_sendable.cpp
        src/handlers/data_channel/data_channel_receivable.cpp
        src/handlers/media_track/a_media_track_handler.cpp
        src/handlers/media_track/recordable.cpp
        src/handlers/media_track/video_recorder.cpp
)
target_link_libraries(handlers PRIVATE
        third_party_lib
        logging
)
target_include_directories(handlers PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_library(security STATIC
        src/security/authenticator_resolver.cpp
        src/security/a_mosaic_authenticator.cpp
        src/security/impl/no_auth_authenticator.cpp
        src/security/impl/simple_token_authenticator.cpp
)
target_link_libraries(security PUBLIC
        third_party_lib
        logging
)
target_include_directories(security PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_library(mosaic-rtc-core STATIC
        src/core/mosaic_connector.cpp
        src/core/mosaic_connector_factory.cpp
        src/core/connector_state_manager.cpp
        src/core/peer_connection_manager.cpp
        src/core/peer_connection_observer.cpp
)
target_link_libraries(mosaic-rtc-core PUBLIC
        third_party_lib
        signaling_server
        logging
        handlers
        security
)

target_include_directories(mosaic-rtc-core PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${JSONCPP_INCLUDE_DIRS}
)

add_library(mosaic-auto-configurer STATIC
        src/auto_configurer/auto_configurer.cpp
        src/auto_configurer/connector_resolver.cpp
        src/config_reader/config_reader_resolver.cpp
        src/config_reader/yaml_config_reader.cpp
        src/auto_configurer/i_configurable_connector.cpp)

target_include_directories(mosaic-auto-configurer PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
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

################################################################################
# Testing Configure
################################################################################
option(BUILD_TESTS "Build tests" ON)

if (BUILD_TESTS)
    find_package(GTest REQUIRED)

    enable_testing()

    add_executable(config_reader_tests
            test/config_reader/test_yaml_config_reader.cpp
            test/config_reader/test_config_reader_resolver.cpp
    )

    add_executable(signaling_tests
            test/signaling/test_websocket_client.cpp
    )
    add_executable(logger_tests
            test/logger/test_logging_level.cpp
    )

    # Link libraries to test executables
    target_link_libraries(config_reader_tests
            mosaic-auto-configurer
            third_party_lib
            GTest::GTest               # Google Test
            GTest::Main                # Google Test main function
    )
    target_link_libraries(signaling_tests
            mosaic-rtc-core          # Main library
            third_party_lib
            GTest::GTest               # Google Test
            GTest::Main                # Google Test main function
    )
    target_link_libraries(logger_tests
            mosaic-rtc-core
            GTest::GTest               # Google Test
            GTest::Main                # Google Test main function
    )

    # Set test header paths
    target_include_directories(signaling_tests PRIVATE
            include
            tests
    )
    target_include_directories(logger_tests PRIVATE
            include
            tests
    )

    # Copy test data files to build directory
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test/config_reader/test_config.yaml
            ${CMAKE_CURRENT_BINARY_DIR}/test/config_reader/test_config.yaml COPYONLY)

    # Google Test discovery and registration
    include(GoogleTest)
    gtest_discover_tests(config_reader_tests)
    gtest_discover_tests(signaling_tests)
    gtest_discover_tests(logger_tests)

    # Register individual tests
    add_test(NAME config_reader_tests COMMAND config_reader_tests)
    add_test(NAME websocket_client_tests COMMAND websocket_client_tests) # Not stable
    add_test(NAME logger_tests COMMAND logger_tests)


endif ()

################################################################################
# Installation Configure
################################################################################
include(GNUInstallDirs)

install(TARGETS mosaic-core mosaic-rtc-core mosaic-auto-configurer logging signaling_server handlers security third_party_lib
        EXPORT mosaic-core_targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT mosaic-core_targets
        FILE mosaic-core_targets.cmake
        NAMESPACE mosaic::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mosaic-core
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/mosaic-core-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/mosaic-core-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mosaic-core
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/mosaic-core-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/mosaic-core-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/mosaic-core-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mosaic-core
)
