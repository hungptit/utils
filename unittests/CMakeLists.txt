PROJECT(TOOLS)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../")
set(EXTERNAL_DIR "${ROOT_DIR}/../3p")
message("ROOT_DIR: ${ROOT_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ENABLE_ALL_WARNINGS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(POSITION_INDEPENDENT_CODE OFF)

# Build hyperscan using the C++17 fork.
include(FetchContent)
FetchContent_Declare(
  hyperscan
  GIT_REPOSITORY git@github.com:hungptit/hyperscan.git
  GIT_TAG develop)
set(BUILD_SHARED_LIBS OFF)
FetchContent_MakeAvailable(hyperscan)
add_library(hs::hs ALIAS hs)
add_library(hs::hs_runtime ALIAS hs_runtime)

FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG devel)
set(BUILD_SHARED_LIBS OFF)
FetchContent_MakeAvailable(catch2)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# CMake build flags
option(UTILS_ENABLE_COVERAGE "Generate coverage" ON)
option(UTILS_ENABLE_WERROR "Enable all warnings as errors" OFF)
option(UTILS_USE_AVX2 "Enable tests for AVX2 implementations" OFF)

include(CheckCXXCompilerFlag)
include(AddCXXCompilerFlag)
include(CXXFeatureCheck)

# Use AVX2 if specified
if (UTILS_USE_AVX2)
  add_cxx_compiler_flag(-DUSE_AVX2)
endif()

add_cxx_compiler_flag(-DFMT_HEADER_ONLY)

ENABLE_TESTING()

set(COMMAND_SRC_FILES timeutils matchers string timestamp exact_matchers regex_matchers random_string)
foreach (src_file ${COMMAND_SRC_FILES})
  ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
  target_link_libraries(${src_file} PRIVATE hs::hs hs::hs_runtime Catch2::Catch2 Catch2::Catch2WithMain)
  target_include_directories(${src_file} PRIVATE ${fmt_SOURCE_DIR}/include/ ${ROOT_DIR}/src ${hyperscan_SOURCE_DIR}/src ${catch2_SOURCE_DIR}/src)
  ADD_TEST(${src_file} ./${src_file})
endforeach (src_file)
