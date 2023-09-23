cmake_minimum_required(VERSION 3.13)

project(m_mem VERSION 1.0.0 DESCRIPTION "Common elements of mlibs.")

option(BUILD_DOC "Build documentation" ON)
option(BUILD_TEST "Build tests" ON)

if (NOT COMPOSITE_BUILD)
    find_library(M_COMMON_LIB NAMES m_common libm_common)
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

add_library(m_mem SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/m_mem.c
            ${CMAKE_CURRENT_SOURCE_DIR}/src/m_alloc.c
            ${CMAKE_CURRENT_SOURCE_DIR}/api/m_mem.h
            ${CMAKE_CURRENT_SOURCE_DIR}/api/m_alloc.h
            ${CMAKE_CURRENT_SOURCE_DIR}/src/system_alloc.c
            ${CMAKE_CURRENT_SOURCE_DIR}/src/fixed_alloc.c
            ${CMAKE_CURRENT_SOURCE_DIR}/src/arena_alloc.c
            ${CMAKE_CURRENT_SOURCE_DIR}/src/slab_alloc.c)
target_include_directories(m_mem
                           PUBLIC
                               $<INSTALL_INTERFACE:api>
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>
                           PRIVATE
                               ${CMAKE_CURRENT_SOURCE_DIR}/inc
                               ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (NOT COMPOSITE_BUILD)
    target_link_libraries(m_mem ${M_COMMON_LIB})
else()
    add_dependencies(m_mem m_common)
    target_link_libraries(m_mem m_common)
endif()

install(TARGETS m_mem DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/api/m_mem.h
              ${CMAKE_CURRENT_SOURCE_DIR}/api/m_alloc.h
        DESTINATION include/m_libs)

if (BUILD_TEST)
    enable_testing()

    add_executable(m_mem_unit_test
                   ${CMAKE_CURRENT_SOURCE_DIR}/test/fixed_alloc.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/test/arena_alloc.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/test/slab_alloc.cpp)

    target_link_libraries(m_mem_unit_test m_mem GTest::gtest GTest::gtest_main)
    if (COMPOSITE_BUILD)
        add_dependencies(m_mem_unit_test m_mem)
    endif()

    include(GoogleTest)
    gtest_discover_tests(m_mem_unit_test)
endif()
