cmake_minimum_required(VERSION 3.14)
set(VERSION_MAJOR 1)
set(VERSION_MINOR 1)
set(VERSION_PATCH 2)
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

project(mosaic-core VERSION ${VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_PYTHON_BINDINGS "Build Python bindings" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_SHARED_LIBS "Build shared libraries instead of static" ON)

# RPATH settings for finding shared libraries at runtime
if (BUILD_SHARED_LIBS)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

if (POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif ()
if (POLICY CMP0148)
    cmake_policy(SET CMP0148 NEW)
endif ()

find_package(cpprestsdk REQUIRED)
find_package(fmt REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(OpenCV REQUIRED)
find_package(jsoncpp REQUIRED)

add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(python)
add_subdirectory(test)

################################################################################
# Installation Configure
################################################################################
include(GNUInstallDirs)

install(TARGETS mosaic-core third_party_lib
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
        ${CMAKE_CURRENT_SOURCE_DIR}/mosaic-core-config.cmake.in
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
