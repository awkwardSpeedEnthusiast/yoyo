ifx_add_library(toolWidget SHARED
    HEADERS
        src/toolModel.hpp
        include/toolwidget.h
    SOURCES
        src/toolModel.cpp
        src/toolwidget.cpp
        src/toolwidget.ui
    DEPENDS
        PUBLIC yoyo::Sdk
        PRIVATE Qt5::Widgets
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    ALIAS yoyo::ToolWidget
    EXPORT_MACRO YOYO_GUI_TOOL_SHARED_EXPORT
)
