my_add_test(plugin_manager_tests
    AUTOMOC
    SOURCE
        PluginManagerTests.cpp
    INCLUDES
      PUBLIC ${CMAKE_SOURCE_DIR}/src/plugin_manager/src
    DEPENDS
        PUBLIC yoyo::PluginManager
        PUBLIC Qt6::Test
        PUBLIC yoyo::Test::Utilities
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
