cmake_minimum_required(VERSION 2.8.10)

set(CMAKE_C_COMPILER gcc)

project(mesh_builder_AdH C)

# Include Directories for header files
include_directories("${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/structs")

# Specify binary and library output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(BUILD_DEBUG_LEVEL "2" CACHE STRING "0: None  1: -g  2: -g + _DEBUG")
if (BUILD_DEBUG_LEVEL LESS 0 OR BUILD_DEBUG_LEVEL GREATER 2)
    message(FATAL_ERROR "BUILD_DEBUG_LEVEL parameter must be an integer value in the range 0 to 2, inclusive")
elseif (BUILD_DEBUG_LEVEL EQUAL 1)
    add_definitions(-g)
elseif (BUILD_DEBUG_LEVEL EQUAL 2)
    add_definitions(-D_DEBUG -g -Wall -Wextra)
endif()

option(USE_COLUMNFIRST "Node numbering is column first, so nodes on boundary 1-4 are numbered first" ON)
if (USE_COLUMNFIRST)
    message("Using column-first numbering by adding definition _COLUMNFIRST")
    add_definitions(-D_COLUMNFIRST)
endif (USE_COLUMNFIRST)

option(TEST_PARABOLIC_BOWL "Parabolic bowl testcase" OFF)
if (TEST_PARABOLIC_BOWL)
    message("Build parabolic bowl bathymetry by definition _PARABOLIC_BOWL")
    add_definitions(-D_PARABOLIC_BOWL)
endif (TEST_PARABOLIC_BOWL)

option(TEST_TRAPEZOID "Trapezoidal cross-section" OFF)
if (TEST_TRAPEZOID)
    message("Build parabolic bowl bathymetry by definition _TRAPEZOID")
    add_definitions(-D_TRAPEZOID)
endif (TEST_TRAPEZOID)

#####################################
# EXE LINKER FLAGS
set(CMAKE_EXE_LINKER_FLAGS "-lm")

add_subdirectory(structs)
add_subdirectory(builder)
add_subdirectory(input)
add_subdirectory(output)
add_subdirectory(main)
