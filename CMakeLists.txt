cmake_minimum_required(VERSION 3.13)

project(mlibs VERSION 1.0.0 DESCRIPTION "A collection of basic C libraries.")

option(BUILD_DOC "Build documentation" ON)
option(BUILD_TEST "Build tests" ON)

set(COMPOSITE_BUILD YES)
add_compile_definitions(COMPOSITE_BUILD=1)

if (BUILD_DOC)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_MAN YES)
    set(DOXYGEN_EXCLUDE_PATTERNS */inc/*)

    find_package(Doxygen
                 REQUIRED dot
                 OPTIONAL_COMPONENTS mscgen dia)

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

option(BUILD_M_COMMON "Enable building m_common" ON)
if(BUILD_M_COMMON)
    add_subdirectory(m_common)
endif()

option(BUILD_M_MEM "Enable building m_mem" ON)
if(BUILD_M_MEM)
    add_subdirectory(m_mem)
endif()

option(BUILD_M_ARGS "Enable building m_args" ON)
if(BUILD_M_ARGS)
    add_subdirectory(m_args)
endif()

option(BUILD_M_HASH "Enable building m_hash" ON)
if(BUILD_M_HASH)
    add_subdirectory(m_hash)
endif()

option(BUILD_M_STR "Enable building m_str" ON)
if(BUILD_M_STR)
    add_subdirectory(m_str)
endif()

option(BUILD_M_LIST "Enable building m_list" ON)
if(BUILD_M_LIST)
    add_subdirectory(m_list)
endif()

option(BUILD_M_MAP "Enable building m_map" ON)
if(BUILD_M_MAP)
    add_subdirectory(m_map)
endif()
