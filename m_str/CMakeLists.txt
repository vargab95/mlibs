cmake_minimum_required(VERSION 3.13)

project(m_str VERSION 1.0.0 DESCRIPTION "Utility library for managing strings.")

option(BUILD_DOC "Build documentation" ON)
option(BUILD_TEST "Build tests" ON)

if (NOT COMPOSITE_BUILD)
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

if (NOT COMPOSITE_BUILD)
    target_link_libraries(m_str ${M_COMMON_LIB} ${M_MEM_LIB})
else()
    add_dependencies(m_str m_common m_mem)
    target_link_libraries(m_str m_common m_mem)
endif()

install(TARGETS m_str DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/api/m_str.h DESTINATION include/m_libs)

if (BUILD_TEST)
    add_executable(m_str_test
                   ${CMAKE_CURRENT_SOURCE_DIR}/test/run_all.c)

    target_link_libraries(m_str_test m_str)

    if (COMPOSITE_BUILD)
        add_dependencies(m_str_test m_str)
    endif()
endif()
