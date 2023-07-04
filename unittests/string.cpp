#include "fmt/format.h"
#include <string>

#include "fmt/format.h"
#include "memchr.hpp"
#include "strcmp.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"

using Catch::Matchers::Equals;

TEST_CASE("strncmp", "SIMD implementations of strncmp") {
    std::string line1("Hello");
    std::string line2("Helle");

    CHECK(strncmp(line1.data(), line2.data(), line1.size()) != 0);
    CHECK(strncmp(line1.data(), line1.data(), line1.size()) == 0);

    CHECK(!utils::sse2::strncmp(line1.data(), line2.data(), line1.size()));
    CHECK(utils::sse2::strncmp(line1.data(), line1.data(), line1.size()));
}

// AVX2 related methods
#ifdef USE_AVX2
TEST_CASE("avx2_memchr", "memchr") {
    const std::string line("A collection of useful C++ utility classes written using C++11/14");

    // Simple tests for memchr
    CHECK(memchr(line.data(), '*', line.size()) == NULL);
    CHECK(memchr(line.data(), '2', line.size()) == NULL);
    CHECK((static_cast<const char *>(memchr(line.data(), '1', line.size())) - line.data()) ==
          60);

    // // Simple tests for utils::avx2::memchr
    CHECK(utils::avx2::memchr(line.data(), '*', line.size()) == nullptr);
    CHECK(utils::avx2::memchr(line.data(), ']', line.size()) == nullptr);

    CHECK((static_cast<const char *>(utils::avx2::memchr(line.data(), '4', line.size())) -
           line.data()) == 64);
    CHECK((static_cast<const char *>(utils::avx2::memchr(line.data(), ' ', line.size())) -
           line.data()) == 1);
    CHECK((static_cast<const char *>(utils::avx2::memchr(line.data(), 'w', line.size())) -
           line.data()) == 43);

    // Simple manual test
    auto ptr = utils::avx2::memchr(line.data(), 'C', line.size());
    fmt::print("Data: {}\n", ptr + 1);
}

TEST_CASE("strncmp_avx2", "SIMD implementations of strncmp") {
    std::string line1("Hello");
    std::string line2("Helle");

    CHECK(!utils::avx2::strncmp(line1.data(), line2.data(), line1.size()));
    CHECK(utils::avx2::strncmp(line1.data(), line1.data(), line1.size()));
}
#endif
