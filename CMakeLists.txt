cmake_minimum_required(VERSION 3.27...4.5)

###################################
# conan settings
list(APPEND CMAKE_PROJECT_TOP_LEVEL_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/config/cmake-conan/conan_provider.cmake)
set(CONAN_BUILD_PROFILE "default;auto-cmake")
set(CONAN_INSTALL_ARGS "--build=missing")

###################################
# C++ settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

###################################
# script directory
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/config/cmake ${CMAKE_CURRENT_LIST_DIR})

project(yoyo 
    VERSION 0.1.0
    DESCRIPTION "A template description" 
    LANGUAGES CXX C
)

set(YOYO_INCLUDE_INSTALL_PATH ${CMAKE_CURRENT_BINARY_DIR}/install/include/)
set(YOYO_BIN_INSTALL_PATH ${CMAKE_CURRENT_BINARY_DIR}/install/bin/)
set(YOYO_LIB_INSTALL_PATH ${CMAKE_CURRENT_BINARY_DIR}/install/lib/)
set(YOYO_DOC_INSTALL_PATH ${CMAKE_CURRENT_BINARY_DIR}/install/doc/)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install/)
set(CMAKE_INSTALL_BINDIR ${YOYO_BIN_INSTALL_PATH})
set(CMAKE_INSTALL_LIBDIR ${YOYO_LIB_INSTALL_PATH})
set(CMAKE_INSTALL_INCLUDEDIR ${YOYO_INCLUDE_INSTALL_PATH})

#my_find_windeployqt()

## coverage flags
include(coverage)
## declarative library and executable definitions
include(utilities)

## libraries and dependencies
include(dependencies)
include(libraries)

## we have tests
include(CTest)
include(testSupport)

add_subdirectory(src)
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

#add_subdirectory(doc)
add_subdirectory(installer)

finalize_coverage()
