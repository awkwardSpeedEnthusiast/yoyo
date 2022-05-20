ifx_add_test(gui_tree_tests
    MOCK QT
    HEADERS
        TreeWidgetFixture.hpp
    SOURCES
        TreeWidgetTests.cpp
        TreeWidgetFixture.cpp
        TreeWidgetDnDTests.cpp
        TreeWidgetSelectionTests.cpp
        TreeWidgetRequestTests.cpp
    DEPENDS
        PUBLIC yoyo::TreeWidget
        PUBLIC yoyo::Nodes
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Test
        PUBLIC yoyo::Test::Utilities
        PUBLIC yoyo::Utilities
        PUBLIC yoyo::Command
)

