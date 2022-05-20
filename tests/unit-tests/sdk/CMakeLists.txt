ifx_add_test(sdk_tests
    MOCK QT
    SOURCES
        node_base_tests.cpp
        node_factory_tests.cpp
        documentation_tests.cpp
        gui_node_tests.cpp
        data_node_tests.cpp
        gui_node_widget_tests.cpp
        protocol_node_tests.cpp
    DEPENDS
        PUBLIC yoyo::Sdk
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Test
        PUBLIC yoyo::Test::Utilities
)

#ifx_add_test(sdk_widget_tests
#    WIDGET
#    SOURCES
#        gui_node_widget_tests.cpp
#    DEPENDS
#        PUBLIC yoyo::Sdk
#)
