my_add_test(dataModel_nodes_tests
    AUTOMOC
    SOURCE
        bitTest.cpp
        buttonTest.cpp
        leafNodeTest.cpp
        checkBoxTest.cpp
        lineEditTest.cpp
        comboBoxTest.cpp
        groupTest.cpp
        factoryTests.cpp
    INCLUDES
        PRIVATE ${CMAKE_SOURCE_DIR}/src/dataModel/nodes/src/basicGui
        PRIVATE ${CMAKE_SOURCE_DIR}/src/dataModel/nodes/src/dataNodes
    DEPENDS
        PUBLIC yoyo::Nodes
        PUBLIC Qt6::Widgets
        PUBLIC Qt6::Test
        PUBLIC yoyo::Test::Utilities
)

