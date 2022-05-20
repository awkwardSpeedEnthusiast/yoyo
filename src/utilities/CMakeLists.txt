ifx_add_library(utilities
    HEADERS
        include/treeUtils.hpp
    SOURCES
        src/treeUtils.cpp
    DEPENDS
        PUBLIC yoyo::Sdk
        PRIVATE Qt5::Widgets
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    INTERFACE_INCLUDES
        include
    ALIAS yoyo::Utilities
)
