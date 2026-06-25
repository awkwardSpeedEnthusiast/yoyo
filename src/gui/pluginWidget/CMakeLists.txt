my_add_library(pluginWidget STATIC
    AUTOMOC AUTOUIC
    HEADER
        include/plugin_widget.h
        src/plugin_model.hpp
        src/plugin_description_dialog.hpp
    SOURCE
        src/plugin_widget.cpp
        src/plugin_model.cpp
        src/plugin_description_dialog.cpp
    FORMS
        src/plugin_widget.ui
        src/plugin_description_dialog.ui    
    DEPENDS
        PUBLIC yoyo::PluginManager
        PRIVATE Qt6::Widgets
    INCLUDES
      PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    ALIAS yoyo::PluginWidget
    EXPORT_MACRO YOYO_GUI_PLUGIN_SHARED_EXPORT
)
