#include "fmt/format.h"
#include <string>

#include "timeutils.hpp"
#include <time.h>

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

const std::string datafile("data.log");

TEST_CASE("Parsing time strings", "Positive") {
    utils::TimeParser parser("%Y-%m-%d %H:%M:%S");
    std::string timestamp("2018-1-11 10:30:05");
    std::time_t t = parser(timestamp.c_str());
    utils::TimePrinter printer("%Y-%m-%d %H:%M:%S");
    printer(t);
    fmt::print("{0} --> {1}\n", timestamp, printer.buffer);
    std::string expected_results("2018-01-11 10:30:05");
    REQUIRE(expected_results == printer(t));
}
