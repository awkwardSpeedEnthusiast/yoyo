ifx_add_executable(yoyo
    HEADERS
        src/yoyo_application.hpp
        src/yoyo_main_window.h
        src/file_management.hpp
    SOURCES
        src/yoyo_application.cpp
        src/yoyo_main_window.cpp
        src/yoyo_main_window.ui
        src/main.cpp
        src/file_management.cpp
    DEPENDS
        PRIVATE yoyo::Sdk
        PRIVATE yoyo::Command
        PRIVATE yoyo::Io
        PRIVATE yoyo::Connection
        PRIVATE yoyo::Nodes
        PRIVATE yoyo::PluginWidget
        PRIVATE yoyo::ToolWidget
        PRIVATE yoyo::PropertyWidget
        PRIVATE yoyo::TreeWidget
        PRIVATE yoyo::PluginManager
    INSTALL
        ${CMAKE_INSTALL_BINDIR}
    DEFINES
        PRIVATE YOYO_VERSION="${YOYO_PROJECT_VERSION}"
)
ifx_deploy_system_libraries(yoyo)
