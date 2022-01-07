cmake_minimum_required(VERSION 3.13)

project(m_str VERSION 1.0.0 DESCRIPTION "Utility library for managing strings.")

option(BUILD_DOC "Build documentation" ON)
option(BUILD_TEST "Build tests" ON)

find_library(M_COMMON_LIB NAMES m_common libm_common)
find_library(M_MEM_LIB NAMES m_mem libm_mem)

add_library(m_str SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/m_str.c
            ${CMAKE_CURRENT_SOURCE_DIR}/api/m_str.h)
target_include_directories(m_str
                           PUBLIC
                               $<INSTALL_INTERFACE:api>
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>
                           PRIVATE
                               ${CMAKE_CURRENT_SOURCE_DIR}/inc
                               ${CMAKE_CURRENT_SOURCE_DIR}/src)
                           target_link_libraries(m_str ${M_COMMON_LIB} ${M_MEM_LIB})

install(TARGETS m_str DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/api/m_str.h DESTINATION include/m_libs)

if (BUILD_TEST)
  add_executable(m_str_test
                 ${CMAKE_CURRENT_SOURCE_DIR}/test/run_all.c)
  target_link_libraries(m_str_test m_str)
endif()
