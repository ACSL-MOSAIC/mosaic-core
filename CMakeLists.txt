cmake_minimum_required(VERSION 3.14)
project(BA_GCS_rtc_sender VERSION 1.0.0)

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
        OpenSSL::SSL
        OpenSSL::Crypto
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
        src/handlers/data_channel/i_data_channel_receivable.cpp
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

add_library(observers STATIC
        src/observers/create_sdp_answer_observer.cpp
        src/observers/data_channel_observer.cpp
        src/observers/peer_connection_observer.cpp
        src/observers/simple_set_local_description_observer.cpp
        src/observers/simple_set_remote_description_observer.cpp
)
target_link_libraries(observers PRIVATE
        third_party_lib
        logging
        handlers
)
target_include_directories(observers PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_library(BA_GCS_rtc_sender STATIC
        src/gcs_connector.cpp
        src/gcs_connector_factory.cpp
        src/connector_state_manager.cpp
        src/peer_connection_manager.cpp
)
target_link_libraries(BA_GCS_rtc_sender PRIVATE
        third_party_lib
)
target_link_libraries(BA_GCS_rtc_sender PUBLIC
        ${JSONCPP_LIBRARIES}
        signaling_server
        logging
        handlers
        observers
)

target_include_directories(BA_GCS_rtc_sender PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${JSONCPP_INCLUDE_DIRS}
)

################################################################################
# Testing Configure
################################################################################
option(BUILD_TESTS "Build tests" ON)

if (BUILD_TESTS)
    find_package(GTest REQUIRED)

    enable_testing()

    add_executable(websocket_client_tests
            test/test_websocket_client.cpp
    )
    add_executable(logger_tests
            test/test_logging_level.cpp
    )

    # 테스트 실행 파일에 라이브러리 링크
    target_link_libraries(websocket_client_tests
            BA_GCS_rtc_sender          # 메인 라이브러리
            third_party_lib
            GTest::GTest               # Google Test
            GTest::Main                # Google Test main 함수
    )
    target_link_libraries(logger_tests
            BA_GCS_rtc_sender
            GTest::GTest               # Google Test
            GTest::Main                # Google Test main 함수
    )

    # 테스트 헤더 경로 설정
    target_include_directories(websocket_client_tests PRIVATE
            include
            tests
    )
    target_include_directories(logger_tests PRIVATE
            include
            tests
    )

    # Google Test 발견 및 등록
    include(GoogleTest)
    gtest_discover_tests(websocket_client_tests)
    gtest_discover_tests(logger_tests)

    # 개별 테스트 등록
    add_test(NAME websocket_client_tests COMMAND websocket_client_tests) # Not stable
    add_test(NAME logger_tests COMMAND logger_tests)


endif ()

################################################################################
# Installation Configure
################################################################################
include(GNUInstallDirs)

install(TARGETS BA_GCS_rtc_sender third_party_lib signaling_server logging handlers observers
        EXPORT ba_gcs_rtc_sender_targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ba_gcs_rtc_sender_targets
        FILE ba_gcs_rtc_sender_targets.cmake
        NAMESPACE ba_gcs::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ba_gcs_rtc_sender
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ba_gcs_rtc_sender-config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/ba_gcs_rtc_sender-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ba_gcs_rtc_sender
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/ba_gcs_rtc_sender-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ba_gcs_rtc_sender-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/ba_gcs_rtc_sender-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ba_gcs_rtc_sender
)