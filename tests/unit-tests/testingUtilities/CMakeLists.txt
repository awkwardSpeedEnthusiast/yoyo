ifx_add_library(yoyo-testing-utilities
    NOCOVERAGE
    HEADERS
        guiFixture.hpp
        gui_node_mock.h
        contextmenu_operator.hpp
        gui_simulator.hpp
    SOURCES
        guiFixture.cpp
        contextmenu_operator.cpp
        gui_simulator.cpp
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS
        PUBLIC yoyo::Sdk
        PUBLIC Qt5::Test
        PUBLIC Qt5::Widgets
        PUBLIC gtest
        PUBLIC gmock
    ALIAS yoyo::Test::Utilities
)
