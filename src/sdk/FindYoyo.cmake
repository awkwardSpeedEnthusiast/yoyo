# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindYoyo
-------

Finds the Yoyo SDK library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``Yoyo::Sdk``
  The Yoyo Sdk library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``Yoyo_FOUND``
  True if the system has the Yoyo Sdk library.
``Yoyo_VERSION``
  The version of the Yoyo Sdk library which was found.
``Yoyo_INCLUDE_DIRS``
  Include directories needed to use Yoyo Sdk.
``Yoyo_LIBRARIES``
  Libraries needed to link to Yoyo Sdk.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``Yoyo_INCLUDE_DIR``
  The directory containing ``yoyo.h``.
``Yoyo_LIBRARY``
  The path to the Yoyo Sdk library.

#]=======================================================================]

find_package(PkgConfig)
pkg_check_modules(PC_Yoyo QUIET Yoyo)

find_path(Yoyo_INCLUDE_DIR
  NAMES yoyo.h
  PATHS ${PC_Yoyo_INCLUDE_DIRS}
  PATH_SUFFIXES Yoyo
)

find_library(Yoyo_LIBRARY_RELEASE
  NAMES yoyo
  PATHS ${PC_Yoyo_LIBRARY_DIRS}/Release
)
find_library(Yoyo_LIBRARY_DEBUG
  NAMES yoyo
  PATHS ${PC_Yoyo_LIBRARY_DIRS}/Debug
)

include(SelectLibraryConfigurations)
select_library_configurations(Yoyo)

set(Yoyo_VERSION ${PC_Yoyo_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Yoyo
  FOUND_VAR Yoyo_FOUND
  REQUIRED_VARS
    Yoyo_LIBRARY
    Yoyo_INCLUDE_DIR
  VERSION_VAR Yoyo_VERSION
)

if(Yoyo_FOUND)
  set(Yoyo_LIBRARIES ${Yoyo_LIBRARY})
  set(Yoyo_INCLUDE_DIRS ${Yoyo_INCLUDE_DIR})
  set(Yoyo_DEFINITIONS ${PC_Yoyo_CFLAGS_OTHER})
endif()

if(Yoyo_FOUND AND NOT TARGET Yoyo::Sdk)
  add_library(Yoyo::Sdk UNKNOWN IMPORTED)
  set_target_properties(Yoyo::Sdk PROPERTIES
    IMPORTED_LOCATION "${Yoyo_LIBRARY}"
    INTERFACE_COMPILE_OPTIONS "${PC_Yoyo_CFLAGS_OTHER}"
    INTERFACE_INCLUDE_DIRECTORIES "${Yoyo_INCLUDE_DIR}"
  )
endif()

if(Yoy_FOUND)
  if (NOT TARGET Yoyo::Sdk)
    add_library(Yoyo::Sdk UNKNOWN IMPORTED)
  endif()
  if (Yoyo_LIBRARY_RELEASE)
    set_property(TARGET Yoyo::Sdk APPEND PROPERTY
      IMPORTED_CONFIGURATIONS RELEASE
    )
    set_target_properties(Yoyo::Sdk PROPERTIES
      IMPORTED_LOCATION_RELEASE "${Yoyo_LIBRARY_RELEASE}"
    )
  endif()
  if (Yoyo_LIBRARY_DEBUG)
    set_property(TARGET Yoyo::Sdk APPEND PROPERTY
      IMPORTED_CONFIGURATIONS DEBUG
    )
    set_target_properties(Yoyo::Sdk PROPERTIES
      IMPORTED_LOCATION_DEBUG "${Yoyo_LIBRARY_DEBUG}"
    )
  endif()
  set_target_properties(Yoyo::Sdk PROPERTIES
    INTERFACE_COMPILE_OPTIONS "${PC_Yoyo_CFLAGS_OTHER}"
    INTERFACE_INCLUDE_DIRECTORIES "${Yoyo_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(
  Yoyo_INCLUDE_DIR
  Yoyo_LIBRARY
)
# compatibility variables
set(Yoyo_VERSION_STRING ${Yoyo_VERSION})
