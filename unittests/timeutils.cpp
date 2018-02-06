#include "fmt/format.h"
#include <string>

#include "timeutils.hpp"
#include <time.h>

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

const std::string datafile("data.log");

TEST_CASE("Parsing time strings", "Positive") {
    utils::TimeParser parser("%Y-%m-%d %H:%M:%S");
    std::time_t t = parser("2018-1-11 10:30:05");
    utils::TimePrinter printer("%Y-%m-%d %H:%M:%S");
    printer(t);
    fmt::print("parsed time: {}\n", printer.buffer);
    std::string expected_results("2018-01-11 10:30:05");
    REQUIRE(expected_results == printer.buffer); // TODO: Need to normalize time.
}

