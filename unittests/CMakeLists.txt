option(UTILS_USE_AVX2 "Enable tests for AVX2 implementations" ON)

ENABLE_TESTING()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include other headers such as fmt library.
include_directories ("../3p/include")

# Include utils headers
include_directories ("../src")
include_directories("../_deps/fmt-src/include/")
include_directories("../_deps/doctest-src/")

set(COMMAND_SRC_FILES timeutils matchers string timestamp exact_matchers regex_matchers random_string)
foreach (src_file ${COMMAND_SRC_FILES})
  ADD_EXECUTABLE(${src_file} ${src_file}.cpp)
  TARGET_LINK_LIBRARIES(${src_file} ${LIB_HS} ${LIB_HS_RUNTIME} -lm fmt::fmt string-algorithms)
  ADD_TEST(${src_file} ./${src_file})
endforeach (src_file)
