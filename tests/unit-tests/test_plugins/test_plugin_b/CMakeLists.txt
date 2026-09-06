my_add_library(test-plugin-b PLUGIN NOCOVERAGE
    AUTOMOC
    HEADER
        test_plugin.hpp
    SOURCE
        test_plugin.cpp
    DEPENDS
        PUBLIC yoyo::Sdk
    ALIAS yoyo::Test::TestPluginB
    INCLUDES
      PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
    EXPORT_MACRO YOYO_TEST_PLUGIN_EXPORT
)
if (WIN32)
set(LIB_PREFIX )
set(PLUGIN_EXTENSION ".dll")
elseif(LINUX)
set(LIB_PREFIX "lib")
set(PLUGIN_EXTENSION ".so")
endif()

configure_file(test_plugin_description.json ${CMAKE_CURRENT_BINARY_DIR}/test_plugin_description.json)
