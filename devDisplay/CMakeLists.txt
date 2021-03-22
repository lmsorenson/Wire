cmake_minimum_required(VERSION 3.5)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13.5" CACHE STRING "Minimum OS X deployment version")
project(devDisplay LANGUAGES CXX VERSION $ENV{DEPLOY_VERSION})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_subdirectory(
    ${PROJECT_SOURCE_DIR}/../WireLib
    lib/
)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(devDisplay SHARED
      main.cpp
      Windows/mainwindow.cpp
      Windows/mainwindow.h
      Windows/mainwindow.ui
      Widgets/EmbeddedDevice.cpp
      Widgets/EmbeddedDevice.h
      Widgets/ErroWidget.cpp
      Widgets/ErrorWidget.h
  )
else()
  add_executable(devDisplay MACOSX_BUNDLE
        main.cpp
        Windows/mainwindow.cpp
        Windows/mainwindow.h
        Windows/mainwindow.ui
        Widgets/EmbeddedDevice.cpp
        Widgets/EmbeddedDevice.h
        Widgets/ErroWidget.cpp
        Widgets/ErrorWidget.h
  )
endif()

target_include_directories(
    devDisplay PUBLIC
    ${PROJECT_SOURCE_DIR}/../WireLib/include
    )
target_link_libraries(devDisplay PUBLIC Wire Qt5::Widgets)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("building package. . .")
    set(prefix "${PROJECT_NAME}.app/Contents")
    set(INSTALL_RUNTIME_DIR "${prefix}/MacOS")
    set(INSTALL_CMAKE_DIR "${prefix}/Resources")

    set(CPACK_BUNDLE_NAME "DeviceDisplay")

    macro(install_qt5_plugin _qt_plugin_name _qt_plugins_var _prefix)
        get_target_property(_qt_plugin_path "${_qt_plugin_name}" LOCATION)
        message("checking if plugin path exists")
        if(EXISTS "${_qt_plugin_path}")
            get_filename_component(_qt_plugin_file "${_qt_plugin_path}" NAME)
            get_filename_component(_qt_plugin_type "${_qt_plugin_path}" PATH)
            get_filename_component(_qt_plugin_type "${_qt_plugin_type}" NAME)
            set(_qt_plugin_dest "${_prefix}/PlugIns/${_qt_plugin_type}")
            message("${_qt_plugin_path} to ${_qt_plugin_dest}")
            install(FILES "${_qt_plugin_path}"
                    DESTINATION "${_qt_plugin_dest}")
            set(${_qt_plugins_var}
                    "${${_qt_plugins_var}};\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${_qt_plugin_dest}/${_qt_plugin_file}")
        else()
            message(FATAL_ERROR "QT plugin ${_qt_plugin_name} not found")
        endif()
    endmacro()

    install_qt5_plugin("Qt5::QCocoaIntegrationPlugin" QT_PLUGINS ${prefix})
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/qt.conf"
            "[Paths]\nPlugins = ${_qt_plugin_dir}\n")
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/qt.conf"
            DESTINATION "${INSTALL_CMAKE_DIR}")

    message ("${CMAKE_INSTALL_PREFIX}")

    # Destination paths below are relative to ${CMAKE_INSTALL_PREFIX}
    install(TARGETS ${PROJECT_NAME}
            BUNDLE DESTINATION . COMPONENT Runtime
            RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR} COMPONENT Runtime)

    # Note Mac specific extension .app
    set(APPS "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app")

    # Directories to look for dependencies
    set(DIRS "${CMAKE_BINARY_DIR}")

    message(STATUS "CMAKE BINARY: ${CMAKE_BINARY_DIR}")
    message(STATUS "CMAKE PREFIX PATH: ${CMAKE_PREFIX_PATH}")

    # Path used for searching by FIND_XXX(), with appropriate suffixes added
    if(CMAKE_PREFIX_PATH)
        foreach(dir ${CMAKE_PREFIX_PATH})
            message(STATUS "${dir}")
            list(APPEND DIRS "${dir}/bin" "${dir}/lib")
        endforeach()
    endif()

    # Append Qt's lib folder which is two levels above Qt5Widgets_DIR
    list(APPEND DIRS "${Qt5Widgets_DIR}/../..")

    include(InstallRequiredSystemLibraries)

    message(STATUS "DESTDIR: $ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}")
    message(STATUS "APPS: ${APPS}")
    message(STATUS "QT_PLUGINS: ${QT_PLUGINS}")
    message(STATUS "DIRS: ${DIRS}")

    install(CODE "include(BundleUtilities)
      fixup_bundle(\"${APPS}\" \"${QT_PLUGINS}\" \"${DIRS}\")")

    set(CPACK_GENERATOR "DRAGNDROP")
    include(CPack)
endif()