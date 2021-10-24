cmake_minimum_required(VERSION 3.13)

project(m_map VERSION 1.0.0 DESCRIPTION "A simple hash table implementation.")

option(BUILD_DOC "Build documentation" ON)
option(BUILD_TEST "Build tests" ON)

find_library(M_HASH_LIB NAMES m_hash libm_hash)
find_library(M_COMMON_LIB NAMES m_common libm_common)
find_library(M_MEM_LIB NAMES m_mem libm_mem)

add_library(m_map SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/m_map.c
            ${CMAKE_CURRENT_SOURCE_DIR}/src/m_map_iterator.c
            ${CMAKE_CURRENT_SOURCE_DIR}/api/m_map.h)
target_include_directories(m_map
                            PUBLIC
                                $<INSTALL_INTERFACE:api>
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>
                            PRIVATE
                                ${CMAKE_CURRENT_SOURCE_DIR}/inc
                                ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(m_map ${M_HASH_LIB} ${M_MEM_LIB} ${M_COMMON_LIB})

install(TARGETS m_map DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/api/m_map.h DESTINATION include/m_libs)

if (BUILD_TEST)
  add_executable(m_map_test
                 ${CMAKE_CURRENT_SOURCE_DIR}/test/run_all.c)
  target_link_libraries(m_map_test m_map)

  add_executable(m_map_speed_test ${CMAKE_CURRENT_SOURCE_DIR}/test/speed_test.cpp)
  target_link_libraries(m_map_speed_test m_map)

  add_test(NAME m_map_functional_test COMMAND ./m_map_test)
  enable_testing()
endif()
