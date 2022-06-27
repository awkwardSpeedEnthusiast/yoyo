ifx_add_library(nodes SHARED
    HEADERS
        include/configuration.hpp
        include/factory_provider.hpp
        src/configuration_data.hpp
        src/data_root.hpp
        src/gui_root.hpp
        src/dataNodes/uint8node.hpp
        src/dataNodes/int8node.hpp
        src/dataNodes/string_node.hpp
        src/dataNodes/bit_node.hpp
        src/dataNodes/data_group.hpp
        src/basicGui/leaf_node.hpp
        src/basicGui/group.hpp
        src/basicGui/button.hpp
        src/basicGui/check_box.hpp
        src/basicGui/line_edit.hpp
        src/basicGui/combo_box.hpp
    SOURCES
        src/configuration.cpp
        src/configuration_data.cpp
        src/factory_provider.cpp
        src/data_root.cpp
        src/gui_root.cpp
        src/dataNodes/uint8node.cpp
        src/dataNodes/int8node.cpp
        src/dataNodes/string_node.cpp
        src/dataNodes/bit_node.cpp
        src/dataNodes/data_group.cpp
        src/basicGui/leaf_node.cpp
        src/basicGui/group.cpp
        src/basicGui/button.cpp
        src/basicGui/check_box.cpp
        src/basicGui/line_edit.cpp
        src/basicGui/combo_box.cpp
        src/node_resources.qrc
    DEPENDS
        PUBLIC yoyo::Sdk
        PRIVATE Qt5::Widgets
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/dataNodes
        ${CMAKE_CURRENT_SOURCE_DIR}/src/basicGui
    ALIAS yoyo::Nodes
    EXPORT_MACRO YOYO_NODES_SHARED_EXPORT
    ADD_TEST_LIBRARY
)
