ifx_add_test(io_tests
    MOCK
    SOURCES
        manager_tests.cpp
    DEPENDS
        PUBLIC yoyo::Io
)

ifx_add_test(json_io_tests
    MOCK
    SOURCES
        json_properties_I_tests.cpp
        json_tests.cpp
        io_fixture.cpp
        io_fixture.hpp
    INCLUDES
        PRIVATE ${CMAKE_SOURCE_DIR}/src/io/include/
        PRIVATE ${CMAKE_SOURCE_DIR}/src/io/src/
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS
        PUBLIC yoyo::Io::Test
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Coverage")
    target_compile_options(json_io_tests PRIVATE ${COVERAGE_COMPILE_FLAGS})
    target_link_options(json_io_tests PRIVATE ${COVERAGE_LINK_FLAGS})
    set(sources_path ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/json_io_tests.dir/__/__/__/src/io/src/json)
    target_sources(coverage_all_files INTERFACE
        ${sources_path}/json_property.cpp.gcno
        ${sources_path}/node_deserializer_json.cpp.gcno
        ${sources_path}/node_serializer_json.cpp.gcno
        )
endif()
