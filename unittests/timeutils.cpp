#include "fmt/format.h"
#include <string>

#include "timeutils.hpp"
#include "timer.hpp"
#include <time.h>

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

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

TEST_CASE("Parsing digits", "Positive") {
	std::string two("12");
	std::string three("345");
	std::string four("1234");
	REQUIRE(std::atoi(two.c_str()) == (utils::timestamp::parse_digits<2>(two.data(), 0)));
	REQUIRE(std::atoi(three.c_str()) == (utils::timestamp::parse_digits<3>(three.data(), 0)));
	REQUIRE(std::atoi(four.c_str()) == (utils::timestamp::parse_digits<4>(four.data(), 0)));
}
