ifx_add_test(dataModel_connection_tests
    MOCK QT
    SOURCES
        connectivityManagerTests.cpp
    DEPENDS
        PUBLIC yoyo::Connection
        PUBLIC yoyo::Nodes
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Test
        PUBLIC yoyo::Test::Utilities
)

