set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_HTML_EXTRA_STYLESHEET ${CMAKE_CURRENT_SOURCE_DIR}/doxygen-awesome.css)
set(DOXYGEN_WARN_IF_UNDOCUMENTED NO)
set(DOXYGEN_QUIET YES)
set(DOXYGEN_IMAGE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

doxygen_add_docs(yoyoDeveloperDocumentation
    ${CMAKE_SOURCE_DIR}/src/sdk/
    ${CMAKE_SOURCE_DIR}/src/command/
    ${CMAKE_SOURCE_DIR}/src/dataModel/
    ${CMAKE_SOURCE_DIR}/src/gui/propertyWidget/
    ${CMAKE_SOURCE_DIR}/src/gui/toolWidget/
    ${CMAKE_SOURCE_DIR}/src/gui/treeWidget/
    ${CMAKE_SOURCE_DIR}/src/io/
    ${CMAKE_SOURCE_DIR}/src/plugin_manager/
    ${CMAKE_CURRENT_SOURCE_DIR}
    #ALL
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/
    COMMENT "Yoyo developer documentation"
)
