ifx_add_library(connection SHARED
    HEADERS
        include/connectivity_manager.hpp
    SOURCES
        src/connectivity_manager.cpp
    DEPENDS
        PUBLIC yoyo::Sdk
        PRIVATE yoyo::Utilities
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    ALIAS yoyo::Connection
    EXPORT_MACRO YOYO_CONNECTION_SHARED_EXPORT
)
