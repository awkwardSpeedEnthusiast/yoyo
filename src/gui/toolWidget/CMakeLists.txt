my_add_library(toolWidget SHARED
    AUTOMOC AUTOUIC
    HEADER
        src/tool_model.hpp
        include/tool_widget.h
    SOURCE
        src/tool_model.cpp
        src/tool_widget.cpp
    FORMS
        src/toolwidget.ui
    DEPENDS
        PUBLIC yoyo::Sdk
        PRIVATE Qt6::Widgets
    INCLUDES
      PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    ALIAS yoyo::ToolWidget
    EXPORT_MACRO YOYO_GUI_TOOL_SHARED_EXPORT
)
