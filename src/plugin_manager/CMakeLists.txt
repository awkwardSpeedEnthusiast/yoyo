ifx_add_library(pluginManager SHARED
    HEADERS
        include/plugin_data.h
        include/plugin_manager.h
        src/plugin_settings.hpp
        src/plugin_data_reader.hpp
    SOURCES
        src/plugin_data.cpp
        src/plugin_manager.cpp
        src/plugin_settings.cpp
        src/plugin_data_reader.cpp
    DEPENDS
        PUBLIC yoyo::Sdk
        PUBLIC Boost::filesystem
    PUBLIC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
    PRIVATE_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    ALIAS yoyo::PluginManager
    EXPORT_MACRO YOYO_PLUGIN_MANAGER_SHARED_EXPORT
    ADD_TEST_LIBRARY
)
