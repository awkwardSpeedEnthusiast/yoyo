my_add_library(helpWidget STATIC
    AUTOMOC AUTOUIC
    HEADER
        include/help_widget.h
    SOURCE
        src/help_widget.cpp        
    FORMS
        src/help_widget.ui
    DEPENDS
        PRIVATE Qt6::Widgets
        PRIVATE Boost::filesystem
    INCLUDES 
      PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    ALIAS yoyo::HelpWidget
    EXPORT_MACRO YOYO_GUI_PLUGIN_SHARED_EXPORT
)
