ifx_add_test(utilities_tests
    MOCK QT
    SOURCES
        treeUtilititesTests.cpp
    DEPENDS
        PUBLIC yoyo::Nodes
        PUBLIC yoyo::Utilities
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Test
        PUBLIC yoyo::Test::Utilities
)

