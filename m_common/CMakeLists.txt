cmake_minimum_required(VERSION 3.13)

project(m_common VERSION 1.0.0 DESCRIPTION "Common elements of mlibs.")

option(BUILD_DOC "Build documentation" ON)
option(BUILD_TEST "Build tests" ON)

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

add_library(m_common SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/src/m_common.c
            ${CMAKE_CURRENT_SOURCE_DIR}/api/m_common.h)
target_include_directories(m_common
                           PUBLIC
                               $<INSTALL_INTERFACE:api>
                               $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>
                           PRIVATE
                               ${CMAKE_CURRENT_SOURCE_DIR}/inc
                               ${CMAKE_CURRENT_SOURCE_DIR}/src)

install(TARGETS m_common DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/api/m_common.h DESTINATION include/m_libs)

if (BUILD_TEST)
  #add_executable(m_common_test
  #               ${CMAKE_CURRENT_SOURCE_DIR}/test/run_all.c)
  #target_link_libraries(m_common_test m_common)
endif()
