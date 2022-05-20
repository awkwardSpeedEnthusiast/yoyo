ifx_add_library(command SHARED
    HEADERS
        include/command.h
        include/command_handler.h
    SOURCES
        src/command.cpp
        src/command_handler.cpp
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    INTERFACE_INCLUDES
        include
    DEPENDS
        PUBLIC Qt5::Core
    ALIAS yoyo::Command
    EXPORT_MACRO YOYO_COMMAND_SHARED_EXPORT
)
