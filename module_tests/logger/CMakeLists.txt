cmake_minimum_required(VERSION 3.5)

add_executable (ModuleTest_Logger
                LoggerModuleTest.cc
                run_tests.cc)

target_link_libraries (ModuleTest_Logger
                       ModuleTest_common
                       mlibs)

add_test (NAME ModuleTests_Logger COMMAND ModuleTest_Logger)
