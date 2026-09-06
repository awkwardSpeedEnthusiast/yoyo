my_add_library(connection SHARED
    AUTOMOC
    HEADER
        include/connectivity_manager.hpp
    SOURCE
        src/connectivity_manager.cpp
    DEPENDS
        PUBLIC yoyo::Sdk
        PRIVATE yoyo::Utilities
    INCLUDES
      PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    ALIAS yoyo::Connection
    EXPORT_MACRO YOYO_CONNECTION_SHARED_EXPORT
)
