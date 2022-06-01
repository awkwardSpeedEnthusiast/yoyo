ifx_add_library(pluginWidget STATIC
    HEADERS
        include/plugin_widget.h
        src/plugin_model.hpp
        src/plugin_description_dialog.hpp
    SOURCES
        src/plugin_widget.cpp
        src/plugin_widget.ui
        src/plugin_model.cpp
        src/plugin_description_dialog.cpp
        src/plugin_description_dialog.ui
    DEPENDS
        PUBLIC yoyo::PluginManager
        PRIVATE Qt5::Widgets
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    ALIAS yoyo::PluginWidget
    EXPORT_MACRO YOYO_GUI_PLUGIN_SHARED_EXPORT
)
