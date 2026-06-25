my_add_test(dataModel_connection_tests
    AUTOMOC
    SOURCE
        connectivityManagerTests.cpp
    DEPENDS
        PUBLIC yoyo::Connection
        PUBLIC yoyo::Nodes
        PUBLIC Qt6::Widgets
        PUBLIC Qt6::Test
        PUBLIC yoyo::Test::Utilities
)

