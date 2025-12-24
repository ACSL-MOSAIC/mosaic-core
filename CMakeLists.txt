cmake_minimum_required(VERSION 3.14)
project(BA_GCS_rtc_sender VERSION 1.0.0)

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

# C++ 17 표준 사용 (WebRTC 요구사항)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################
# 서드파티 라이브러리 포함
################################################################################
include(cmake/ThirdPartyLibs.cmake)

# 서드파티 라이브러리가 제대로 설정되었는지 확인
if (NOT THIRD_PARTY_READY)
    message(FATAL_ERROR "Third party libraries not properly configured!")
endif ()

################################################################################
# WebRTC Sender Library
################################################################################
add_library(logging STATIC
        src/logger/log_service.cpp
)
target_include_directories(logging PUBLIC
        include
)

add_library(signaling_server STATIC
        src/websocket/websocket_client.cpp
)
target_link_libraries(signaling_server PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        logging
        cpprestsdk::cpprest
)
target_include_directories(signaling_server PUBLIC
        include
        ${CPPREST_INCLUDE_DIRS}
)

add_library(BA_GCS_rtc_sender STATIC
        src/signaling_server.cpp
        src/robot_webrtc_client.cpp
        src/robot_webrtc_client_factory.cpp
        src/client_state_manager.cpp
        src/peer_connection_manager.cpp
        src/logger/webrtc_log.cpp
        src/handlers/data_channel/a_data_channel_handler.cpp
        src/handlers/data_channel/data_channel_sendable.cpp
        src/handlers/data_channel/i_data_channel_receivable.cpp
        src/handlers/media_track/media_track_handler.cpp
        src/handlers/media_track/recordable.cpp
        src/handlers/media_track/video_recorder.cpp
        src/observers/create_sdp_answer_observer.cpp
        src/observers/data_channel_observer.cpp
        src/observers/peer_connection_observer.cpp
        src/observers/simple_set_local_description_observer.cpp
        src/observers/simple_set_remote_description_observer.cpp

        # 헤더 파일들
        include/rtc_sender.h
)
# 컴파일 시 헤더 경로와 정의 설정
target_link_libraries(BA_GCS_rtc_sender PRIVATE
        third_party_lib
)
target_link_libraries(BA_GCS_rtc_sender PUBLIC
        ${JSONCPP_LIBRARIES}
        signaling_server
        logging
)

target_include_directories(BA_GCS_rtc_sender PUBLIC
        include
        ${JSONCPP_INCLUDE_DIRS}
)

################################################################################
# 테스트 설정
################################################################################
# 테스트 빌드 옵션
option(BUILD_TESTS "Build tests" ON)

if (BUILD_TESTS)
    # Google Test 찾기
    find_package(GTest REQUIRED)

    # 테스트 활성화
    enable_testing()

    # 테스트 실행 파일 생성
    add_executable(websocket_client_tests
            test/test_websocket_client.cpp
    )
    add_executable(logger_tests
            test/test_logging_level.cpp
    )

    # 테스트 실행 파일에 라이브러리 링크
    target_link_libraries(websocket_client_tests
            rtc_sender                  # 메인 라이브러리
            third_party_lib
            GTest::GTest               # Google Test
            GTest::Main                # Google Test main 함수
    )
    target_link_libraries(logger_tests
            rtc_sender
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
# 설치 설정
################################################################################
install(TARGETS
        BA_GCS_rtc_sender
        DESTINATION lib/${PROJECT_NAME})

# 헤더 파일 설치
install(DIRECTORY include/
        DESTINATION include/
)

