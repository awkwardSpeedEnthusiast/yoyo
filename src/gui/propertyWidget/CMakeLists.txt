my_add_library(propertyWidget SHARED
    AUTOMOC AUTOUIC
    HEADER
        include/property_widget.h
        src/property_input/property_input.hpp
        src/property_input/number_input.hpp
        src/property_input/qstring_input.hpp
        src/property_input/invisible_string_input.hpp
        src/property_input/connected_string_input.hpp
        src/property_input/pattern_string_input.hpp
        src/property_input/connection_in_input.hpp
        src/property_input/connection_out_input.hpp
        src/property_input/connection_input.hpp
        src/property_input/text_input.hpp
        src/property_input/bool_input.hpp
        src/property_input/enumeration_input.hpp
        src/property_input/layout_input.hpp
        src/property_input/connected_boolean_input.hpp
        src/property_input/script_input.hpp
        src/property_input/enum_input.hpp
        src/property_input/property_input_factory.hpp
    SOURCE
        src/property_widget.cpp
        src/property_input/property_input.cpp
        src/property_input/number_input.cpp
        src/property_input/qstring_input.cpp
        src/property_input/invisible_string_input.cpp
        src/property_input/connected_string_input.cpp
        src/property_input/pattern_string_input.cpp
        src/property_input/connection_in_input.cpp
        src/property_input/connection_out_input.cpp
        src/property_input/connection_input.cpp
        src/property_input/text_input.cpp
        src/property_input/bool_input.cpp
        src/property_input/enumeration_input.cpp
        src/property_input/layout_input.cpp
        src/property_input/connected_boolean_input.cpp
        src/property_input/script_input.cpp
        src/property_input/enum_input.cpp
        src/property_input/property_input_factory.cpp
    FORMS
        src/property_input/text_input.ui
        src/property_input/connection_input.ui
        src/property_widget.ui
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
    ALIAS yoyo::PropertyWidget
    EXPORT_MACRO YOYO_GUI_PROPERTY_SHARED_EXPORT
)
