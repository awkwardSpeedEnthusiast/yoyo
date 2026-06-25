my_add_library(treeWidget SHARED
    AUTOMOC AUTOUIC
    HEADER
        src/tree_model.hpp
        include/tree_widget.h
        src/data_adapter.hpp
        src/contextmenu_handler.hpp
    SOURCE
        src/tree_model.cpp
        src/tree_widget.cpp
        src/data_adapter.cpp
        src/contextmenu_handler.cpp
    FORMS
        src/treewidget.ui
    DEPENDS
        PUBLIC yoyo::Sdk
        PUBLIC Qt6::Widgets
        PRIVATE yoyo::Utilities
        PRIVATE yoyo::Command
    INCLUDES
      PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    ALIAS yoyo::TreeWidget
    EXPORT_MACRO YOYO_GUI_TREE_SHARED_EXPORT
)
