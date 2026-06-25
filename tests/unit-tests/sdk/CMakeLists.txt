my_add_test(sdk_tests
    AUTOMOC
    SOURCE
        node_base_tests.cpp
        node_factory_tests.cpp
        documentation_tests.cpp
        gui_node_tests.cpp
        data_node_tests.cpp
        gui_node_widget_tests.cpp
        protocol_node_tests.cpp
    DEPENDS
        PUBLIC yoyo::Sdk
        PUBLIC Qt6::Widgets
        PUBLIC Qt6::Test
        PUBLIC yoyo::Test::Utilities
)

#my_add_test(sdk_widget_tests
#    WIDGET
#    SOURCE
#        gui_node_widget_tests.cpp
#    DEPENDS
#        PUBLIC yoyo::Sdk
#)
