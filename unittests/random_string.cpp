#include "fmt/format.h"
#include <string>

#include "random_string.hpp"
#include <time.h>

#define CATCH_CONFIG_MAIN
#include "catch2/catch_test_macros.hpp"

TEST_CASE("Constructor", "Basic tests") {
    utils::RandomStringGenerator gen1;
    utils::RandomStringGenerator gen2;
    fmt::print("Random string: {}\n", gen1(16));
    fmt::print("Random string: {}\n", gen2(16));
    CHECK(gen1(16) !=
          gen2(16)); // Check that we genereate different random string for different seeds.
    fmt::print("Random string: {}\n", gen1(128));
}
