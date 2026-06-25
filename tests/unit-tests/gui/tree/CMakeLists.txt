my_add_test(gui_tree_tests
    AUTOMOC
    HEADER
        TreeWidgetFixture.hpp
    SOURCE
        TreeWidgetTests.cpp
        TreeWidgetFixture.cpp
        TreeWidgetDnDTests.cpp
        TreeWidgetSelectionTests.cpp
        TreeWidgetRequestTests.cpp
    DEPENDS
        PUBLIC yoyo::TreeWidget
        PUBLIC yoyo::Nodes
        PUBLIC Qt6::Widgets
        PUBLIC Qt6::Test
        PUBLIC yoyo::Test::Utilities
        PUBLIC yoyo::Utilities
        PUBLIC yoyo::Command
)

