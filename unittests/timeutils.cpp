#include "fmt/format.h"
#include <string>

#include "timeutils.hpp"
#include "timer.hpp"
#include <time.h>

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

using Catch::Matchers::Equals;

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

TEST_CASE("Parse timestamp", "scribe timestamp") {
	std::string timestamp("02/04/2018 23:42:22");
	utils::timestamp::scribe parser;
	std::time_t t = parser(timestamp.data());

	// Print out the parsed results
	utils::TimePrinter printer("%Y-%m-%d %H:%M:%S");
    printer(t);
    fmt::print("{0} --> {1}\n", timestamp, printer.buffer);
	CHECK_THAT(printer.buffer, Equals("2018-02-04 23:42:22"));
}

TEST_CASE("Comparators", "timestamp") {
	std::string timestamp1("01/04/2018 13:42:22");
	std::string timestamp2("02/04/2018 23:00:22");
	std::string timestamp3("02/04/2018 15:31:00");
	utils::timestamp::scribe parser;
	std::time_t t1 = parser(timestamp1.data());
	std::time_t t2 = parser(timestamp2.data());
	std::time_t t3 = parser(timestamp3.data());

	// Print out the parsed results
	utils::TimePrinter printer("%Y-%m-%d %H:%M:%S");

	printer(t1);
    fmt::print("{0} --> {1}\n", timestamp1, printer.buffer);
	CHECK_THAT(printer.buffer, Equals("2018-01-04 13:42:22"));

	printer(t2);
    fmt::print("{0} --> {1}\n", timestamp2, printer.buffer);
	CHECK_THAT(printer.buffer, Equals("2018-02-04 23:00:22"));
	
	printer(t3);
    fmt::print("{0} --> {1}\n", timestamp3, printer.buffer);
	CHECK_THAT(printer.buffer, Equals("2018-02-04 15:31:00"));

	// Check all time constraints.
	utils::timestamp::AllTimestamps all;
	utils::timestamp::OlderThan older_than(t3);
	utils::timestamp::NewerThan newer_than(t3);
	utils::timestamp::Between between(t1, t2);
	utils::timestamp::Equals equals(t3);
	CHECK(all(t1));

	CHECK(!older_than(t1));
	CHECK(older_than(t2));

	CHECK(!newer_than(t2));
	CHECK(newer_than(t1));

	CHECK(!equals(t2));
	CHECK(equals(t3));

	CHECK(!between(t2));
	CHECK(between(t3));
}
