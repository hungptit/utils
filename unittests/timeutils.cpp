#include "fmt/format.h"
#include <string>

#include "timer.hpp"
#include "timeutils.hpp"
#include "timestamp.hpp"
#include "timestamp_experiments.hpp"
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
    REQUIRE(std::atoi(two.c_str()) == (utils::parse_digits<2>(two.data(), 0)));
    REQUIRE(std::atoi(three.c_str()) == (utils::parse_digits<3>(three.data(), 0)));
    REQUIRE(std::atoi(four.c_str()) == (utils::parse_digits<4>(four.data(), 0)));
}

TEST_CASE("Parse timestamp", "scribe timestamp") {
    std::string timestamp("02/04/2018 23:42:22");
    utils::experiments::scribe parser;
    std::time_t t = parser(timestamp.data());

    // Print out the parsed results
    utils::TimePrinter printer("%Y-%m-%d %H:%M:%S");
    printer(t);
    fmt::print("{0} --> {1}\n", timestamp, printer.buffer);
    CHECK_THAT(printer.buffer, Equals("2018-02-04 23:42:22"));
}

TEST_CASE("Very fast timestamp parser", "scribe") {
    std::string timestamp("02/04/2018 23:42:22");
    utils::experiments::scribe parser;
    auto t = utils::parse_timestamp<utils::Timestamp>(timestamp.data());
    fmt::print("sizeof(t): {}\n", sizeof(t));
    fmt::print("Parsed data: {0}-{1}-{2} {3}:{4}:{5}\n", t.tm_mon(), t.tm_mday(), t.tm_year(),
               t.tm_hour(), t.tm_min(), t.tm_sec());
    // Print out the parsed results
    // fmt::print("{0} --> {1}\n", timestamp, printer.buffer);
    // CHECK_THAT(printer.buffer, Equals("2018-02-04 23:42:22"));
}

TEST_CASE("Comparators", "timestamp") {
    std::string timestamp1("01/04/2018 13:42:22");
    std::string timestamp2("02/04/2018 23:00:22");
    std::string timestamp3("02/04/2018 15:31:00");
    utils::experiments::scribe parser;
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
    utils::AllTimestamps<std::time_t> all;
    utils::OlderThan<std::time_t> older_than(t3);
    utils::NewerThan<std::time_t> newer_than(t3);
    utils::Between<std::time_t> between(t1, t2);
    utils::Equals<std::time_t> equals(t3);
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

TEST_CASE("utils::Timestamp", "") {
	using value_type = utils::Timestamp;
	value_type tm;
	utils::TimePrinter printer("%m-%d-%Y %H:%M:%S");
	value_type t1(2000, 1, 3, 10, 10, 9);
	value_type t2(2018, 2, 3, 5, 21, 49);
	value_type t3(2018, 2, 5, 0, 1, 3);
	
	CHECK(t1 == t1);
	CHECK_FALSE(t1 == t2);

	CHECK(t1 != t2);
	CHECK_FALSE(t1 != t1);

	CHECK(t1 < t2);
	CHECK_FALSE(t2 < t1);
	
	CHECK(t3 > t2);
	CHECK_FALSE(t2 > t3);

	CHECK(t3 < utils::MAX_TIME);
	CHECK(t3 > utils::MIN_TIME);
}
