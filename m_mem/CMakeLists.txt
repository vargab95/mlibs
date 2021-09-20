cmake_minimum_required(VERSION 3.13)

project(m_mem VERSION 1.0.0 DESCRIPTION "Common elements of mlibs.")

option(BUILD_DOC "Build documentation" ON)
option(BUILD_TEST "Build tests" ON)

find_library(M_COMMON_LIB NAMES m_common libm_common)

add_library(m_mem SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/m_mem.c
            ${CMAKE_CURRENT_SOURCE_DIR}/api/m_mem.h)
target_include_directories(m_mem
                           PUBLIC
                               $<INSTALL_INTERFACE:api>
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>
                           PRIVATE
                               ${CMAKE_CURRENT_SOURCE_DIR}/inc
                               ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(m_mem ${M_COMMON_LIB})

install(TARGETS m_mem DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/api/m_mem.h DESTINATION include/m_libs)

if (BUILD_TEST)
  #add_executable(m_mem_test
  #               ${CMAKE_CURRENT_SOURCE_DIR}/test/run_all.c)
  #target_link_libraries(m_mem_test m_mem)
endif()
