ifx_add_library(treeWidget SHARED
    HEADERS
        src/tree_model.hpp
        include/treewidget.h
        src/data_adapter.hpp
        src/contextmenu_handler.hpp
    SOURCES
        src/tree_model.cpp
        src/treewidget.cpp
        src/treewidget.ui
        src/data_adapter.cpp
        src/contextmenu_handler.cpp
    DEPENDS
        PUBLIC yoyo::Sdk
        PUBLIC Qt5::Widgets
        PRIVATE yoyo::Utilities
        PRIVATE yoyo::Command
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    ALIAS yoyo::TreeWidget
    EXPORT_MACRO YOYO_GUI_TREE_SHARED_EXPORT
)
