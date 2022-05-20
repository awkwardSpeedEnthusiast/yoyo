ifx_add_test(dataModel_nodes_tests
    MOCK QT
    SOURCES
        bitTest.cpp
        buttonTest.cpp
        leafNodeTest.cpp
        checkBoxTest.cpp
        lineEditTest.cpp
        comboBoxTest.cpp
        groupTest.cpp
        factoryTests.cpp
    DEPENDS
        PUBLIC yoyo::Nodes::Test
        PUBLIC Qt5::Widgets
        PUBLIC Qt5::Test
        PUBLIC yoyo::Test::Utilities
)

