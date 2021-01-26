cmake_policy(SET CMP0048 NEW)
project(
  string-algorithms
  VERSION 0.1.0
  LANGUAGES CXX)
cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-pedantic)
add_compile_options(-pedantic-errors)
add_compile_options(-static)

# Install required libraries
include(FetchContent)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

set(POSITION_INDEPENDENT_CODE ON)

# Fetch required libraries

# Add doctest
FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/onqtam/doctest.git
  GIT_TAG 2.4.4
  GIT_SHALLOW TRUE)

set(DOCTEST_WITH_TESTS OFF)
FetchContent_MakeAvailable(doctest)

# Add benchmark
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.5.2
  GIT_SHALLOW TRUE)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_ENABLE_ASSEMBLY_TEST OFF)
FetchContent_MakeAvailable(benchmark)

# fmt
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 7.1.3)
set(BUILD_TESTING OFF)

set(FMT_DOC OFF)
set(FMT_TEST OFF)
FetchContent_MakeAvailable(fmt)

# Add cmake-scripts
FetchContent_Declare(
  cmake_scripts
  GIT_REPOSITORY https://github.com/StableCoder/cmake-scripts.git
  GIT_TAG main
  GIT_SHALLOW TRUE)

FetchContent_Populate(cmake_scripts)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/_deps/cmake_scripts-src/;")
include(c++-standards)
include(code-coverage)
include(sanitizers)
include(tools)
include(dependency-graph)

# Used libraries
SET(LIB_HS "${CMAKE_SOURCE_DIR}/3p/lib/libhs.a")
SET(LIB_HS_RUNTIME "${CMAKE_SOURCE_DIR}/3p/lib/libhs_runtime.a")

add_subdirectory(src)
add_subdirectory(unittests)

# Install all header files to destination folder.
if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    install(DIRECTORY src/ DESTINATION include/utils)
endif()

enable_testing()
