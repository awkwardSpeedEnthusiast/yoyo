set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_HTML_EXTRA_STYLESHEET ${CMAKE_CURRENT_SOURCE_DIR}/doxygen-awesome.css)

doxygen_add_docs(yoyoDeveloperDocumentation
    ${CMAKE_SOURCE_DIR}/src/sdk/include/
    ${CMAKE_SOURCE_DIR}/src/command/include/
    ${CMAKE_SOURCE_DIR}/src/dataModel/nodes/include/
    ${CMAKE_SOURCE_DIR}/src/dataModel/connection/include/
    ${CMAKE_SOURCE_DIR}/src/gui/propertyEditor/include/
    ${CMAKE_SOURCE_DIR}/src/gui/toolWidget/include/
    ${CMAKE_SOURCE_DIR}/src/gui/treeDisplay/include/
    ${CMAKE_SOURCE_DIR}/src/io/include/
    ${CMAKE_SOURCE_DIR}/src/plugin_manager/include/
    mainpage.dox
    modules.dox
    properties.dox
    #ALL
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/
    COMMENT "Yoyo developer documentation"
)
