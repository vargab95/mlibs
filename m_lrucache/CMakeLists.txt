cmake_minimum_required(VERSION 3.13)

project(m_lrucache VERSION 1.0.0 DESCRIPTION "Simple LRU cache implementation.")

option(BUILD_DOC "Build documentation" ON)
option(BUILD_TEST "Build tests" ON)

if (NOT COMPOSITE_BUILD)
    find_library(M_COMMON_LIB NAMES m_common libm_common)
    find_library(M_MEM_LIB NAMES m_mem libm_mem)
    find_library(M_HASH_LIB NAMES m_hash libm_hash)
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

add_library(m_lrucache SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/m_lrucache.c
            ${CMAKE_CURRENT_SOURCE_DIR}/api/m_lrucache.h)
target_include_directories(m_lrucache
                           PUBLIC
                               $<INSTALL_INTERFACE:api>
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>
                           PRIVATE
                               ${CMAKE_CURRENT_SOURCE_DIR}/inc
                               ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (NOT COMPOSITE_BUILD)
    target_link_libraries(m_lrucache ${M_COMMON_LIB} ${M_MEM_LIB} ${M_HASH_LIB})
else()
    add_dependencies(m_lrucache m_common m_mem m_hash)
    target_link_libraries(m_lrucache m_common m_mem m_hash)
endif()

install(TARGETS m_lrucache DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/api/m_lrucache.h DESTINATION include/m_libs)

if (BUILD_TEST)
    enable_testing()

    add_executable(m_lrucache_unit_test
                   ${CMAKE_CURRENT_SOURCE_DIR}/test/run_all.cpp)

    target_link_libraries(m_lrucache_unit_test m_lrucache GTest::gtest GTest::gtest_main)
    if (COMPOSITE_BUILD)
        add_dependencies(m_lrucache_unit_test m_lrucache)
    endif()

    include(GoogleTest)
    gtest_discover_tests(m_lrucache_unit_test)
endif()
