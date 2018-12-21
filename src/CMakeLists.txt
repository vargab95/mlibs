cmake_minimum_required(VERSION 3.9)

add_subdirectory ("logger")

set(MLIBS_SRCS
    ${MLIBS_LOGGER_SRCS}
    PARENT_SCOPE
)

include_directories (".")
