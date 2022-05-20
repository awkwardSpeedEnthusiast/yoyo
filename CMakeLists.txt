cmake_minimum_required(VERSION 3.21)

project(yoyo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)
include(utilities)
include(CTest)

find_package(Qt5 COMPONENTS Core Widgets Xml Network Qml Concurrent REQUIRED)
# Boost filesystem is necessary due to a bug in gcc 8 filesystem library on windows, which is
# currently used for yoyo. This bug is meant to be fixed with gcc 9, so when switching to gcc 9, the
# boost::filesystem objects should be replaced with the ones from the standard library.
find_package(Boost 1.78.0 COMPONENTS filesystem REQUIRED)

set(YOYO_INCLUDE_INSTALL_PATH ${CMAKE_CURRENT_BINARY_DIR}/install/include/)
set(YOYO_BIN_INSTALL_PATH ${CMAKE_CURRENT_BINARY_DIR}/install/bin/)
set(YOYO_LIB_INSTALL_PATH ${CMAKE_CURRENT_BINARY_DIR}/install/lib/)
set(YOYO_DOC_INSTALL_PATH ${CMAKE_CURRENT_BINARY_DIR}/install/doc/)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install/bin/)
set(CMAKE_INSTALL_BINDIR ${YOYO_BIN_INSTALL_PATH})
set(CMAKE_INSTALL_LIBDIR ${YOYO_LIB_INSTALL_PATH})
set(CMAKE_INSTALL_INCLUDEDIR ${YOYO_INCLUDE_INSTALL_PATH})

if (${CMAKE_BUILD_TYPE} STREQUAL "Coverage")
    message(STATUS "Starting coverage build")
    include(coverage)
endif()

ifx_find_windeployqt()
## Automatically process qrc-files, ui-files and use the moc on
#    header files defining Q_OBJECT macros
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(src)

if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

add_subdirectory(doc)

if (${CMAKE_BUILD_TYPE} STREQUAL "Coverage")
    message(STATUS "Starting coverage build")
    add_final_coverage_target()
endif()
