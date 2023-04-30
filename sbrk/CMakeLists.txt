###################################################################################################
##
##      Information about the project
##
###################################################################################################

cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME sbrk)
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES C)

###################################################################################################
##
##      Compilation options
##
###################################################################################################

add_compile_options(
        -Werror

        -Wall
        -Wextra
        -Wpedantic

        -Wcast-align
        -Wcast-qual

        -Wconversion
        -Wfloat-equal
        -Wredundant-decls
        -Wsign-conversion
)

###################################################################################################
##
##      Project options
##
###################################################################################################
set(SOURCES main.c sbrk.c)
        
add_executable(${PROJECT_NAME} ${SOURCES})
