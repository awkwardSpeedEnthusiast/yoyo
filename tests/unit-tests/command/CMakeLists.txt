ifx_add_test(command_tests
    MOCK
    SOURCES
        CommandTests.cpp
        CommandHandlerTests.cpp
    DEPENDS
        PUBLIC yoyo::Command
)
