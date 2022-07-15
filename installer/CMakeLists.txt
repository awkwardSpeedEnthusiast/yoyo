if (WIN32)
    if (NOT INNO_SETUP_PATH)
        message(WARNING "Installer path not specified! Cannot add installer target.")
    else()
        find_program(innosetup ISCC.exe PATHS ${INNO_SETUP_PATH})
        if (NOT innosetup)
            message(WARNING "Could not find Inno Setup. Cannot add installer target.")
        else()
            configure_file(yoyo.iss.in ${CMAKE_CURRENT_BINARY_DIR}/yoyo.iss)
            add_custom_target(installer
                "${innosetup}" "${CMAKE_CURRENT_BINARY_DIR}/yoyo.iss"
                DEPENDS yoyo.iss.in
                COMMENT "Creating Yoyo installer"
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            )
        endif()
    endif()
endif()
