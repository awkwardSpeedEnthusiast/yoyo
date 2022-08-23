ifx_add_library(helpWidget STATIC
    HEADERS
        include/help_widget.h
    SOURCES
        src/help_widget.cpp
        src/help_widget.ui
    DEPENDS
        PRIVATE Qt5::Widgets
        PRIVATE Boost::filesystem
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    ALIAS yoyo::HelpWidget
    EXPORT_MACRO YOYO_GUI_PLUGIN_SHARED_EXPORT
)
