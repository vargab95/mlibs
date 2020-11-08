cmake_minimum_required(VERSION 3.13)

project(m_list VERSION 1.0.0 DESCRIPTION "A simple double linked list implementation.")

option(BUILD_DOC "Build documentation" ON)
option(BUILD_TEST "Build tests" ON)

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
target_link_libraries(m_list m_mem m_common)

if (BUILD_TEST)
  add_executable(m_list_test
                 ${CMAKE_CURRENT_SOURCE_DIR}/test/run_all.c)
  target_link_libraries(m_list_test m_list)

  add_executable(m_list_speed_test ${CMAKE_CURRENT_SOURCE_DIR}/test/speed_test.cpp)
  target_link_libraries(m_list_speed_test m_list)
endif()
