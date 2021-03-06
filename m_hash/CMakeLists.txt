cmake_minimum_required(VERSION 3.13)

project(m_hash VERSION 1.0.0 DESCRIPTION "Collection of hash function implementations.")

option(BUILD_DOC "Build documentation" ON)
option(BUILD_TEST "Build tests" ON)

find_library(M_COMMON_LIB NAMES m_common libm_common)

add_library(m_hash SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/m_hash.c
            ${CMAKE_CURRENT_SOURCE_DIR}/api/m_hash.h)
target_include_directories(m_hash
                           PUBLIC
                               $<INSTALL_INTERFACE:api>
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>
                           PRIVATE
                               ${CMAKE_CURRENT_SOURCE_DIR}/inc
                               ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(m_hash ${M_COMMON_LIB})

install(TARGETS m_hash DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/api/m_hash.h DESTINATION include/m_libs)

if (BUILD_TEST)
  add_executable(m_hash_test
                 ${CMAKE_CURRENT_SOURCE_DIR}/test/run_all.c)
  target_link_libraries(m_hash_test m_hash)
endif()
