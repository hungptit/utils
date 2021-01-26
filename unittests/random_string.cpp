#include "fmt/format.h"
#include <string>

#include "random_string.hpp"
#include <time.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

TEST_CASE("Constructor") {
    utils::RandomStringGenerator gen1;
    utils::RandomStringGenerator gen2;
    fmt::print("Random string: {}\n", gen1(16));
    fmt::print("Random string: {}\n", gen2(16));
    CHECK(gen1(16) !=
          gen2(16)); // Check that we genereate different random string for different seeds.
    fmt::print("Random string: {}\n", gen1(128));
}
