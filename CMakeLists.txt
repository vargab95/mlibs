cmake_minimum_required (VERSION 3.5)

option(MLIBS_BUILD_TESTS "Switch to build test or not")

include_directories ("./src")

add_subdirectory (src)

add_library (mlibs SHARED ${MLIBS_SRCS})

if (MLIBS_BUILD_TESTS)
  add_subdirectory (module_tests)
endif ()
enable_testing ()
