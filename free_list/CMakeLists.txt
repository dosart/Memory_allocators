###################################################################################################
##
##      Information about the project
##
###################################################################################################

cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME free_list)
set (CMAKE_C_STANDARD 11)
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
set(SOURCES free_list_test.c free_list.c)
set(HEADERS free_list.h)
        
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
