
my_add_library(Sdk SHARED
    AUTOMOC
    HEADER
        include/yoyo/property_types.h
        include/yoyo/documentation.h
        include/yoyo/node_base.h
        include/yoyo/node_factory.h
        include/yoyo/data_node.h
        include/yoyo/data_value_node.h
        include/yoyo/data_container_node.h
        include/yoyo/gui_node.h
        include/yoyo/protocol_node.h
        include/yoyo/communication_node.h
        include/yoyo/yoyo_plugin.h
        include/yoyo/documentation_utilities.h
        src/gui_node_widget.hpp
    SOURCE
        src/node_base.cpp
        src/documentation.cpp
        src/node_factory.cpp
        src/data_node.cpp
        src/data_value_node.cpp
        src/data_container_node.cpp
        src/protocol_node.cpp
        src/gui_node.cpp
        src/gui_node_widget.cpp
        src/documentation_utilities.cpp
    DEPENDS
        PUBLIC Qt6::Core
        PRIVATE Qt6::Widgets
        PUBLIC Boost::headers
    INCLUDES
      PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
    INSTALL_INCLUDES
        include
    ALIAS yoyo::Sdk
)
include(GenerateExportHeader)
generate_export_header( Sdk
          EXPORT_MACRO_NAME YOYO_SDK_SHARED_EXPORT
          EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/yoyo/yoyo_sdk_global.h
)

install(TARGETS Sdk EXPORT YoyoSdkConfig
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
install(
  DIRECTORY
    include/
  DESTINATION ${YOYO_INCLUDE_INSTALL_PATH}
  COMPONENT
    Devel
)
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/yoyo/yoyo_sdk_global.h"
  DESTINATION ${YOYO_INCLUDE_INSTALL_PATH}/yoyo/
  COMPONENT
    Devel
)


include(CMakePackageConfigHelpers)
set(ConfigPackageLocation ${YOYO_LIB_INSTALL_PATH}/cmake/yoyo)
set(Yoyo_PREFIX  ${CMAKE_BINARY_DIR}/install)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/Yoyo-version.cmake"
 VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
 COMPATIBILITY SameMajorVersion
)
install(
 FILES
   ${CMAKE_CURRENT_BINARY_DIR}/Yoyo-version.cmake
 DESTINATION
   ${ConfigPackageLocation}
 COMPONENT
   Devel
)

export(TARGETS Sdk NAMESPACE Yoyo::
 FILE ${CMAKE_CURRENT_BINARY_DIR}/YoyoSdkConfig.cmake
)
install(EXPORT YoyoSdkConfig
 FILE
   YoyoSdkConfig.cmake
 NAMESPACE
   Yoyo::
 DESTINATION
   ${ConfigPackageLocation}
)
