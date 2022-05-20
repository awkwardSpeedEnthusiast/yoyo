ifx_add_library(Io SHARED
    HEADERS
        include/io_manager.h
        include/node_serializer.h
        include/node_deserializer.h
        src/json/node_serializer_json.hpp
        src/json/node_deserializer_json.hpp
        src/json/json_property.hpp
    SOURCES
        src/io_manager.cpp
        src/json/node_serializer_json.cpp
        src/json/node_deserializer_json.cpp
        src/json/json_property.cpp
    DEPENDS
        PUBLIC yoyo::Sdk
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    INTERFACE_INCLUDES
        include
    ALIAS yoyo::Io
    EXPORT_MACRO YOYO_IO_SHARED_EXPORT
    ADD_TEST_LIBRARY
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/json/json_constants.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/json_constants.hpp)
