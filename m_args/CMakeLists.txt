cmake_minimum_required(VERSION 3.13)

project(m_args VERSION 1.0.0 DESCRIPTION "A simple tree implementation mainly for storing pathes.")

option(BUILD_DOC "Build documentation" ON)
option(BUILD_TEST "Build tests" ON)

find_library(M_COMMON_LIB NAMES m_common libm_common)
find_library(M_MEM_LIB NAMES m_mem libm_mem)
find_library(M_LIST_LIB NAMES m_list libm_list)

add_library(m_args SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/m_args.c
            ${CMAKE_CURRENT_SOURCE_DIR}/api/m_args.h)
target_include_directories(m_args
                            PUBLIC
                                $<INSTALL_INTERFACE:api>
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>
                            PRIVATE
                                ${CMAKE_CURRENT_SOURCE_DIR}/inc
                                ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(m_args ${M_MEM_LIB} ${M_COMMON_LIB} ${M_LIST_LIB})

install(TARGETS m_args DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/api/m_args.h DESTINATION include/m_libs)

if (BUILD_TEST)
    # add_executable(m_args_test
    #                ${CMAKE_CURRENT_SOURCE_DIR}/test/run_all.c)
    # target_link_libraries(m_args_test m_args)

    # add_executable(m_args_speed_test ${CMAKE_CURRENT_SOURCE_DIR}/test/speed_test.cpp)
    # target_link_libraries(m_args_speed_test m_args)
endif()
