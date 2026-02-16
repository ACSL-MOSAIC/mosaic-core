################################################################################
# Testing Configure
################################################################################
if (BUILD_TESTS)
    find_package(GTest REQUIRED)

    enable_testing()

    add_executable(config_reader_tests
            config_reader/test_yaml_config_reader.cpp
            config_reader/test_config_reader_resolver.cpp
    )

    add_executable(signaling_tests
            signaling/test_websocket_client.cpp
    )
    add_executable(logger_tests
            logger/test_logging_level.cpp
    )

    # Link libraries to test executables
    target_link_libraries(config_reader_tests
            mosaic-core              # Unified library
            GTest::GTest               # Google Test
            GTest::Main                # Google Test main function
    )
    target_link_libraries(signaling_tests
            mosaic-core              # Unified library
            GTest::GTest               # Google Test
            GTest::Main                # Google Test main function
    )
    target_link_libraries(logger_tests
            mosaic-core              # Unified library
            GTest::GTest               # Google Test
            GTest::Main                # Google Test main function
    )

    # Set test header paths
    #    target_include_directories(signaling_tests PRIVATE
    #            ${CMAKE_CURRENT_SOURCE_DIR}/../include
    #    )
    #    target_include_directories(logger_tests PRIVATE
    #            ${CMAKE_CURRENT_SOURCE_DIR}/../include
    #    )

    # Copy test data files to build directory
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config_reader/test_config.yaml
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