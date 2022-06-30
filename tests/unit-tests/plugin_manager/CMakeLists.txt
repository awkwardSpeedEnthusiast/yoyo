ifx_add_test(plugin_manager_tests
    MOCK QT
    SOURCES
        PluginManagerTests.cpp
    DEPENDS
        PUBLIC yoyo::PluginManager::Test
        PUBLIC Qt5::Test
        PUBLIC yoyo::Test::Utilities
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
