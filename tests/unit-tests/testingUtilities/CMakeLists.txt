my_add_library(yoyo-testing-utilities
    STATIC
    AUTOMOC
    NOCOVERAGE
    HEADER
        guiFixture.hpp
        gui_node_mock.h
        contextmenu_operator.hpp
        gui_simulator.hpp
        dialog_operator.hpp
        qt_core_fixture.hpp
    SOURCE
        guiFixture.cpp
        contextmenu_operator.cpp
        gui_simulator.cpp
        dialog_operator.cpp
        qt_core_fixture.cpp
    INCLUDES
      PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS
        PUBLIC yoyo::Sdk
        PUBLIC Qt6::Test
        PUBLIC Qt6::Widgets
        PUBLIC gtest
        PUBLIC gmock
    ALIAS yoyo::Test::Utilities
)
