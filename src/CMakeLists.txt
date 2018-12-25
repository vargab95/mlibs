cmake_minimum_required(VERSION 3.5)

add_subdirectory ("logger")

set(MLIBS_SRCS
    ${MLIBS_LOGGER_SRCS}
    PARENT_SCOPE
)

include_directories (".")
