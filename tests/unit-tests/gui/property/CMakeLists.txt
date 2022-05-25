ifx_add_test(gui_property_tests
    MOCK QT
    HEADERS
        PropertyWidgetFixture.hpp
    SOURCES
        PropertyWidgetFixture.cpp
        PropertyWidgetStringsTests.cpp
        PropertyWidgetNumberTests.cpp
        PropertyWidgetLimitedNumberTests.cpp
        PropertyWidgetConnectionTests.cpp
        PropertyWidgetMixedTests.cpp
        PropertyWidgetEnumTests.cpp
        PropertyWidgetUndoTests.cpp
    DEPENDS
        PUBLIC yoyo::PropertyWidget
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Test
        PUBLIC yoyo::Test::Utilities
        PUBLIC yoyo::Utilities
        PUBLIC yoyo::Command
)

