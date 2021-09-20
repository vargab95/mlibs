cmake_minimum_required(VERSION 3.13)

project(m_list VERSION 1.0.0 DESCRIPTION "A simple double linked list implementation.")

option(BUILD_DOC "Build documentation" ON)
option(BUILD_TEST "Build tests" ON)

find_library(M_MEM_LIB NAMES m_mem libm_mem)
find_library(M_MEM_COMMON NAMES m_common libm_common)

add_library(m_list SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/m_list.c
            ${CMAKE_CURRENT_SOURCE_DIR}/api/m_list.h)
target_include_directories(m_list
                            PUBLIC
                                $<INSTALL_INTERFACE:api>
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>
                            PRIVATE
                                ${CMAKE_CURRENT_SOURCE_DIR}/inc
                                ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(m_list ${M_MEM_LIB} ${M_COMMON_LIB})

install(TARGETS m_list DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/api/m_list.h DESTINATION include/m_libs)

if (BUILD_TEST)
  add_executable(m_list_test
                 ${CMAKE_CURRENT_SOURCE_DIR}/test/run_all.c)
  target_link_libraries(m_list_test m_list)

  add_executable(m_list_speed_test ${CMAKE_CURRENT_SOURCE_DIR}/test/speed_test.cpp)
  target_link_libraries(m_list_speed_test m_list)
endif()
