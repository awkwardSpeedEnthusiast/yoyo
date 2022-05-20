ifx_add_test(gui_tool_tests
    MOCK QT
    SOURCES
        ToolWidgetTests.cpp
    DEPENDS
        PUBLIC yoyo::ToolWidget
        PUBLIC yoyo::Nodes
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Test
        PUBLIC yoyo::Test::Utilities
)

