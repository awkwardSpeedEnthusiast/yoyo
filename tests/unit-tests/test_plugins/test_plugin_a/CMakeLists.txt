ifx_add_library(test-plugin-a PLUGIN NOCOVERAGE
    HEADERS
        test_plugin.hpp
    SOURCES
        test_plugin.cpp
    DEPENDS
        PUBLIC yoyo::Sdk
    ALIAS yoyo::Test::TestPluginA
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
    EXPORT_MACRO YOYO_TEST_PLUGIN_EXPORT
)
configure_file(test_plugin_description.json ${CMAKE_CURRENT_BINARY_DIR}/test_plugin_description.json COPYONLY)
