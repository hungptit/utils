PROJECT(TOOLS)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../")
set(EXTERNAL_DIR "${ROOT_DIR}/../3p")
message("ROOT_DIR: ${ROOT_DIR}")

# set(CMAKE_INSTALL_PREFIX "${ROOT_DIR}")
set(CMAKE_INSTALL_PREFIX "$ENV{HOME}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# CMake build flags
option(UTILS_ENABLE_COVERAGE "Generate coverage" ON)
option(UTILS_ENABLE_WERROR "Enable all warnings as errors" OFF)
option(UTILS_USE_AVX2 "Enable tests for AVX2 implementations" OFF)

# Read the git tags to determine the project version
# include(GetGitVersion)
# get_git_version(GIT_VERSION)

# # Tell the user what versions we are using
# string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" VERSION ${GIT_VERSION})
# message("-- Version: ${VERSION}")

# The version of the libraries
# set(GENERIC_LIB_VERSION ${VERSION})
# string(SUBSTRING ${VERSION} 0 1 GENERIC_LIB_SOVERSION)

include(CheckCXXCompilerFlag)
include(AddCXXCompilerFlag)
include(CXXFeatureCheck)

# Use libc++  provided by llvm
# set(CMAKE_CXX_COMPILER "clang++")
# add_cxx_compiler_flag(-stdlib=libc++)

if (UTILS_ENABLE_COVERAGE)
  set(CMAKE_BUILD_TYPE Debug)
  add_cxx_compiler_flag(-ftest-coverage)
  add_cxx_compiler_flag(-fprofile-arcs)
else()
  set (CMAKE_BUILD_TYPE Release)
  add_cxx_compiler_flag(-O3)
  add_cxx_compiler_flag(-march=native)
endif()

message("Build type: ${CMAKE_BUILD_TYPE}")

add_cxx_compiler_flag(-std=c++14)
add_cxx_compiler_flag(-Wall)

# Use AVX2 if specified
if (UTILS_USE_AVX2)
  add_cxx_compiler_flag(-DUSE_AVX2)
endif()

if (UTILS_ENABLE_WERROR)
  add_cxx_compiler_flag(-pedantic-errors)
endif()

add_cxx_compiler_flag(-Wextra)
add_cxx_compiler_flag(-pedantic)
add_cxx_compiler_flag(-Wshadow)

add_cxx_compiler_flag(-Werror RELEASE)
add_cxx_compiler_flag(-Werror RELWITHDEBINFO)
add_cxx_compiler_flag(-Werror MINSIZEREL)

add_cxx_compiler_flag(-Wshorten-64-to-32)
add_cxx_compiler_flag(-Wfloat-equal)
add_cxx_compiler_flag(-Wint-to-void-pointer-cast)

add_cxx_compiler_flag(-fstrict-aliasing)
if (HAVE_CXX_FLAG_FSTRICT_ALIASING)
  add_cxx_compiler_flag(-Wstrict-aliasing)
endif()
add_cxx_compiler_flag(-Wthread-safety)
if (HAVE_WTHREAD_SAFETY)
  add_definitions(-DHAVE_WTHREAD_SAFETY)
  cxx_feature_check(THREAD_SAFETY_ATTRIBUTES)
endif()

add_cxx_compiler_flag(-DFMT_HEADER_ONLY)
add_cxx_compiler_flag(-DCEREAL_RAPIDJSON_HAS_CXX11_RVALUE_REFS)

ENABLE_TESTING()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Used libraries
SET(LIB_HS "${EXTERNAL_DIR}/lib/libhs.a")
SET(LIB_HS_RUNTIME "${EXTERNAL_DIR}/lib/libhs_runtime.a")

# Include other headers such as fmt library.
include_directories ("${EXTERNAL_DIR}/include")

# Include utils headers
include_directories ("${ROOT_DIR}/src")

set(COMMAND_SRC_FILES timeutils matchers string timestamp regex_matchers random_string)
foreach (src_file ${COMMAND_SRC_FILES})
  ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
  TARGET_LINK_LIBRARIES(${src_file} ${LIB_HS} ${LIB_HS_RUNTIME} -lm)
  # TARGET_LINK_LIBRARIES(${src_file} ${LIB_HS} ${LIB_HS_RUNTIME} -lpthread -lm -fprofile-arcs -ftest-coverage gcov)
  ADD_TEST(${src_file} ./${src_file})
endforeach (src_file)
