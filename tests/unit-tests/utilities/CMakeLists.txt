my_add_test(utilities_tests
    AUTOMOC
    SOURCE
        treeUtilititesTests.cpp
    DEPENDS
        PUBLIC yoyo::Nodes
        PUBLIC yoyo::Utilities
        PUBLIC Qt6::Widgets
        PUBLIC Qt6::Test
        PUBLIC yoyo::Test::Utilities
)

