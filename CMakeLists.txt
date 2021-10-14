cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(m_libs)

add_subdirectory(m_common)
add_subdirectory(m_mem)
add_subdirectory(m_hash)
add_subdirectory(m_map)
add_subdirectory(m_list)
add_subdirectory(m_args)
