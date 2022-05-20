set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_HTML_EXTRA_STYLESHEET ${CMAKE_CURRENT_SOURCE_DIR}/doxygen-awesome.css)

doxygen_add_docs(developerDocumentation
    ${CMAKE_SOURCE_DIR}/src/sdk/include/
    ${CMAKE_CURRENT_SOURCE_DIR}
    mainpage.dox
    nodes.dox
    plugin_creation.dox
    properties.dox
    #ALL
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/sdk/
    COMMENT "Plugin developer documentation"
)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/ DESTINATION ${YOYO_DOC_INSTALL_PATH}/developer/)
