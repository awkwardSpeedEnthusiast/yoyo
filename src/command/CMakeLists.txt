my_add_library(command SHARED
    AUTOMOC
    HEADER
        include/command.h
        include/command_handler.h
    SOURCE
        src/command.cpp
        src/command_handler.cpp
    INCLUDES
      PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    INSTALL_INCLUDES
        include
    DEPENDS
        PUBLIC Qt6::Core
    ALIAS yoyo::Command
    EXPORT_MACRO YOYO_COMMAND_SHARED_EXPORT
)
