my_add_library(utilities STATIC
    HEADER
        include/tree_utils.hpp
    SOURCE
        src/tree_utils.cpp
    DEPENDS
        PUBLIC yoyo::Sdk
        PRIVATE Qt6::Widgets
    INCLUDES
      PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    INSTALL_INCLUDES
        include
    ALIAS yoyo::Utilities
)
