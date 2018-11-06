PROJECT(TOOLS)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../")
set(EXTERNAL_DIR "${ROOT_DIR}/../3p")
message("ROOT_DIR: ${ROOT_DIR}")

# set(CMAKE_INSTALL_PREFIX "${ROOT_DIR}")
set(CMAKE_INSTALL_PREFIX "$ENV{HOME}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# CMake build flags
option(UTILS_ENABLE_COVERAGE "Generate coverage" OFF)
option(UTILS_ENABLE_WERROR "Enable all warnings as errors" OFF)
option(UTILS_USE_AVX2 "Enable tests for AVX2 implementations" OFF)
option(UTILS_ENABLE_FLTO "Enable link time optimization" ON)

include(CheckCXXCompilerFlag)
include(AddCXXCompilerFlag)
include(CXXFeatureCheck)

# Use libc++  provided by llvm
# set(CMAKE_CXX_COMPILER "clang++")
# add_cxx_compiler_flag(-stdlib=libc++)

# set(CMAKE_BUILD_TYPE Debug)

set (CMAKE_BUILD_TYPE Release)
add_cxx_compiler_flag(-O3)
add_cxx_compiler_flag(-march=native)

add_cxx_compiler_flag(-std=c++14)
add_cxx_compiler_flag(-Wall)
add_cxx_compiler_flag(-flto)

# Use AVX2 if specified
if (UTILS_USE_AVX2)
  add_cxx_compiler_flag(-DUSE_AVX2)
endif()

if (UTILS_ENABLE_WERROR)
  add_cxx_compiler_flag(-pedantic-errors)
endif()

# add_cxx_compiler_flag(-Wextra)
# add_cxx_compiler_flag(-Wshadow)
# add_cxx_compiler_flag(-Werror RELEASE)
# add_cxx_compiler_flag(-Werror RELWITHDEBINFO)
# add_cxx_compiler_flag(-Werror MINSIZEREL)

# add_cxx_compiler_flag(-pedantic)

add_cxx_compiler_flag(-fstrict-aliasing)
if (HAVE_CXX_FLAG_FSTRICT_ALIASING)
  add_cxx_compiler_flag(-Wstrict-aliasing)
endif()
# add_cxx_compiler_flag(-Wthread-safety)
if (HAVE_WTHREAD_SAFETY)
  add_definitions(-DHAVE_WTHREAD_SAFETY)
  cxx_feature_check(THREAD_SAFETY_ATTRIBUTES)
endif()

add_cxx_compiler_flag(-DFMT_HEADER_ONLY)
add_cxx_compiler_flag(-DCEREAL_RAPIDJSON_HAS_CXX11_RVALUE_REFS)
add_cxx_compiler_flag(-flto)

ENABLE_TESTING()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include other headers such as fmt library.
include_directories ("${EXTERNAL_DIR}/include")

# Include utils headers
include_directories ("${ROOT_DIR}/src")

# Link time optimisation
if (BENCHMARK_ENABLE_LTO)
  add_cxx_compiler_flag(-flto)
  if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    find_program(GCC_AR gcc-ar)
    if (GCC_AR)
      set(CMAKE_AR ${GCC_AR})
    endif()
    find_program(GCC_RANLIB gcc-ranlib)
    if (GCC_RANLIB)
      set(CMAKE_RANLIB ${GCC_RANLIB})
    endif()
  elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    include(llvm-toolchain)
  endif()
endif()


# Used libraries
SET(LIB_BENCHMARK "${EXTERNAL_DIR}/lib/libbenchmark.a")
set(COMMAND_SRC_FILES string timeutils memchr lookuptable)
foreach (src_file ${COMMAND_SRC_FILES})
  ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
  TARGET_LINK_LIBRARIES(${src_file} -lpthread -lm ${LIB_BENCHMARK})
endforeach (src_file)
