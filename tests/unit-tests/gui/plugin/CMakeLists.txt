my_add_test(gui_plugin_tests
    MOCK QT
    HEADER
    SOURCE
        PluginWidgetTests.cpp
    DEPENDS
        PUBLIC yoyo::PluginWidget
        PUBLIC Qt6::Widgets
        PUBLIC Qt6::Test
        PUBLIC yoyo::Test::Utilities
)

