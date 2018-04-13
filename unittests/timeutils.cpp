#include "fmt/format.h"
#include <string>

#include "timer.hpp"
#include "timeutils.hpp"
#include "timestamp.hpp"
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

TEST_CASE("Very fast timestamp parser", "scribe") {
    std::string timestamp("02/04/2018 23:42:22");
    auto t = utils::parse_scribe_timestamp<utils::Timestamp>(timestamp.data());
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
    
    auto t1 = utils::parse_scribe_timestamp<utils::Timestamp>(timestamp1.data());
    auto t2 = utils::parse_scribe_timestamp<utils::Timestamp>(timestamp2.data());
    auto t3 = utils::parse_scribe_timestamp<utils::Timestamp>(timestamp3.data());

    // Print out the parsed results
    utils::TimePrinter printer("%Y-%m-%d %H:%M:%S");

    fmt::print("{0} --> {1}\n", timestamp1, printer(t1.to_tm()));
    CHECK_THAT(printer(t1.to_tm()), Equals("2018-01-04 13:42:22"));

    fmt::print("{0} --> {1}\n", timestamp2, printer(t2.to_tm()));
    CHECK_THAT(printer(t2.to_tm()), Equals("2018-02-04 23:00:22"));

    fmt::print("{0} --> {1}\n", timestamp3, printer(t3.to_tm()));
    CHECK_THAT(printer(t3.to_tm()), Equals("2018-02-04 15:31:00"));

    // Check all time constraints.
    utils::All<utils::Timestamp> all;
    utils::OlderThan<utils::Timestamp> older_than(t3);
    utils::NewerThan<utils::Timestamp> newer_than(t3);
    utils::Between<utils::Timestamp> between(t1, t2);
    utils::Equals<utils::Timestamp> equals(t3);

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
