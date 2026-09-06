my_add_test(gui_tool_tests
    AUTOMOC
    SOURCE
        ToolWidgetTests.cpp
    DEPENDS
        PUBLIC yoyo::ToolWidget
        PUBLIC yoyo::Nodes
        PUBLIC Qt6::Widgets
        PUBLIC Qt6::Test
        PUBLIC yoyo::Test::Utilities
)

