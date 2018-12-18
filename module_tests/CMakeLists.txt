cmake_minimum_required(VERSION 3.9)

include_directories (${mlibs_INCLUDE_DIRS}
                     ".")

add_subdirectory (common)
add_subdirectory (logger)

add_executable (ModuleTest
                run_tests.cc)
target_link_libraries (ModuleTest
                       ModuleTest_common
                       ModuleTest_logger
                       mlibs)

