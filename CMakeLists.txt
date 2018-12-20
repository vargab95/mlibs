cmake_minimum_required (VERSION 3.9)

project(mlibs VERSION 0.1.0 DESCRIPTION "My self-compiled libs")

include_directories ("./src")

add_subdirectory (src)
add_subdirectory (module_tests)

add_library (mlibs SHARED src/mlibs.cc)
target_link_libraries (mlibs mlibs_logger)

enable_testing ()
add_test (NAME ModuleTests COMMAND Test)
