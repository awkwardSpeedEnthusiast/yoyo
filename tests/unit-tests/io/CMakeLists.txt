my_add_test(io_tests
    AUTOMOCK
    SOURCE
        manager_tests.cpp
    DEPENDS
        PUBLIC yoyo::Io
)

my_add_test(json_io_tests
    AUTOMOC
    SOURCE
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
        PUBLIC yoyo::Io
)

