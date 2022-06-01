ifx_add_test(gui_plugin_tests
    MOCK QT
    HEADERS
    SOURCES
        PluginWidgetTests.cpp
    DEPENDS
        PUBLIC yoyo::PluginWidget
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Test
        PUBLIC yoyo::Test::Utilities
)

