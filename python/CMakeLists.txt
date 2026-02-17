# Python bindings
if (BUILD_PYTHON_BINDINGS)
    find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
    find_package(pybind11 CONFIG REQUIRED)

    set(PYTHON_DEV_DIR ${CMAKE_CURRENT_BINARY_DIR}/mosaic_core)

    pybind11_add_module(_mosaic_core
            bindings/main.cpp
            bindings/bind_logging.cpp
            bindings/bind_configs.cpp
            bindings/bind_handlers.cpp
            bindings/bind_data_channel.cpp
            bindings/bind_media_track.cpp
            bindings/bind_core.cpp
            bindings/bind_auto_configurer.cpp
    )

    target_link_libraries(_mosaic_core PRIVATE
            mosaic-core
    )
    target_include_directories(_mosaic_core PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )

    add_custom_command(TARGET _mosaic_core POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory ${PYTHON_DEV_DIR}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/mosaic_core/__init__.py
            ${PYTHON_DEV_DIR}/__init__.py
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:_mosaic_core>
            ${PYTHON_DEV_DIR}/$<TARGET_FILE_NAME:_mosaic_core>
            COMMENT "Copying Python module to build directory for development"
    )

    if (DEFINED Python3_SITELIB)
        set(PYTHON_INSTALL_DIR ${Python3_SITELIB}/mosaic_core)
    else ()
        set(PYTHON_INSTALL_DIR
                lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages/mosaic_core)
    endif ()

    install(TARGETS _mosaic_core
            LIBRARY DESTINATION ${PYTHON_INSTALL_DIR}
            COMPONENT python
    )

    # Python 패키지 파일들도 설치
    install(FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/mosaic_core/__init__.py
            DESTINATION ${PYTHON_INSTALL_DIR}
            COMPONENT python
    )

    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
            ${CMAKE_CURRENT_BINARY_DIR}/setup.py
            @ONLY
    )

endif ()