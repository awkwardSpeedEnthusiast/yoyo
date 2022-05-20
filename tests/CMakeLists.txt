###############################################################
# Use GTest
#  see https://google.github.io/googletest/quickstart-cmake.html for more details.
#

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/16f637fbf4ffc3f7a01fa4eceb7906634565242f.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Populate(googletest)
add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)

include(GoogleTest)

find_package(Qt5 COMPONENTS Test REQUIRED)
###############################################################
# YoYo tests

add_subdirectory(unit-tests)
