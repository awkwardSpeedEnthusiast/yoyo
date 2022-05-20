add_subdirectory(test_plugin)

ifx_add_test(plugin_manager_tests
    MOCK QT
    SOURCES
        PluginManagerTests.cpp
    DEPENDS
        PUBLIC yoyo::PluginManager::Test
        PUBLIC Qt5::Test
        PUBLIC yoyo::Test::Utilities
)
