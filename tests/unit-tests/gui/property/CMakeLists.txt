my_add_test(gui_property_tests
    AUTOMOC
    HEADER
        PropertyWidgetFixture.hpp
        TestingNodes.hpp
    SOURCE
        PropertyWidgetFixture.cpp
        PropertyWidgetStringsTests.cpp
        PropertyWidgetConnectionTests.cpp
        PropertyWidgetMixedTests.cpp
        PropertyWidgetEnumTests.cpp
        PropertyWidgetUndoTests.cpp
        PropertyWidgetTypesTests.cpp
    INCLUDES
        PRIVATE ${CMAKE_SOURCE_DIR}/src/gui/propertyWidget/src/property_input
    DEPENDS
        PUBLIC yoyo::PropertyWidget
        PUBLIC Qt6::Widgets
        PUBLIC Qt6::Test
        PUBLIC yoyo::Test::Utilities
        PUBLIC yoyo::Utilities
        PUBLIC yoyo::Command
)

