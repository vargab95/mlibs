cmake_minimum_required(VERSION 3.13)

project(m_map VERSION 1.0.0 DESCRIPTION "A simple hash table implementation.")

option(BUILD_DOC "Build documentation" ON)
option(BUILD_TEST "Build tests" ON)

if (NOT COMPOSITE_BUILD)
    find_library(M_HASH_LIB NAMES m_hash libm_hash)
    find_library(M_COMMON_LIB NAMES m_common libm_common)
    find_library(M_MEM_LIB NAMES m_mem libm_mem)
endif()

if (NOT TARGET doc AND BUILD_DOC)
    find_package(Doxygen
                 REQUIRED dot
                 OPTIONAL_COMPONENTS mscgen dia)

    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN YES)

    doxygen_add_docs(
        doc
        ${PROJECT_SOURCE_DIR}
        COMMENT "Generate documentation"
    )
endif()

if (BUILD_TEST)
    enable_testing()
    find_package(GTest REQUIRED)
endif()

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

if (NOT COMPOSITE_BUILD)
    target_link_libraries(m_map ${M_HASH_LIB} ${M_MEM_LIB} ${M_COMMON_LIB})
else()
    add_dependencies(m_map m_hash m_mem m_common)
    target_link_libraries(m_map m_hash m_mem m_common)
endif()

install(TARGETS m_map DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/api/m_map.h DESTINATION include/m_libs)

if (BUILD_TEST)
    enable_testing()

    add_executable(m_map_unit_test
                   ${CMAKE_CURRENT_SOURCE_DIR}/test/read_write.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/test/iterator.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/test/delete.cpp)

    target_link_libraries(m_map_unit_test m_map GTest::gtest GTest::gtest_main)
    if (COMPOSITE_BUILD)
        add_dependencies(m_map_unit_test m_map)
    endif()

    include(GoogleTest)
    gtest_discover_tests(m_map_unit_test)

    add_executable(m_map_speed_test
                   ${CMAKE_CURRENT_SOURCE_DIR}/test/speed.cpp)

    target_link_libraries(m_map_speed_test m_map GTest::gtest GTest::gtest_main)
    if (COMPOSITE_BUILD)
        add_dependencies(m_map_speed_test m_map)
    endif()

    include(GoogleTest)
    gtest_discover_tests(m_map_speed_test)
endif()
