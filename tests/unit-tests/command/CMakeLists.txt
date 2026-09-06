my_add_test(command_tests
    MOCK
    SOURCE
        CommandTests.cpp
        CommandHandlerTests.cpp
    DEPENDS
        PUBLIC yoyo::Command
)
