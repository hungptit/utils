#include "matchers.hpp"
#include "fmt/format.h"
#include "regex_matchers.hpp"
#include "timestamp.hpp"
#include "timeutils.hpp"
#include <cstring>
#include <string>
#include <time.h>

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"
using Catch::Matchers::Equals;

TEST_CASE("StartsWith", "baseline") {
    const std::string pattern("This");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");
    const char *foo = NULL;
    utils::baseline::StartsWith cons(pattern);
    CHECK(cons(line1));
    CHECK(cons(line2));
    CHECK(!cons(line3));
}

TEST_CASE("Contains", "baseline") {
    const std::string pattern("the");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");
    const char *foo = NULL;
    utils::baseline::Contains cons(pattern);
    CHECK(cons(line1));
    CHECK(cons(line2));
    CHECK(!cons(line3));
}

TEST_CASE("Equals", "baseline") {
    const std::string pattern("My first line!");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");
    const char *foo = NULL;
    utils::baseline::Equals cons(pattern);
    CHECK(!cons(line1));
    CHECK(!cons(line2));
    CHECK(cons(line3));
}

TEST_CASE("StartsWith-sse2", "") {
    const std::string pattern("This");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");
    const char *foo = NULL;
    utils::sse2::StartsWith cons(pattern);
    CHECK(cons(line1));
    CHECK(cons(line2));
    CHECK(!cons(line3));
}

TEST_CASE("Contains-sse2", "") {
    const std::string pattern("the");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");
    const char *foo = NULL;
    utils::sse2::Contains cons(pattern);
    CHECK(cons(line1));
    CHECK(cons(line2));
    CHECK(!cons(line3));
}

TEST_CASE("Equals-sse2", "") {
    const std::string pattern("My first line!");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");
    const char *foo = NULL;
    utils::baseline::Equals cons(pattern);
    CHECK(!cons(line1));
    CHECK(!cons(line2));
    CHECK(cons(line3));
}

TEST_CASE("StartsWith-avx2", "") {
    const std::string pattern("This");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");
    const char *foo = NULL;
    utils::sse2::StartsWith cons(pattern);
    CHECK(cons(line1));
    CHECK(cons(line2));
    CHECK(!cons(line3));
}

TEST_CASE("Contains-avx2", "") {
    const std::string pattern("the");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");
    const char *foo = NULL;
    utils::sse2::Contains cons(pattern);
    CHECK(cons(line1));
    CHECK(cons(line2));
    CHECK(!cons(line3));
}

TEST_CASE("Equals-avx2", "") {
    const std::string pattern("My first line!");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");
    const char *foo = NULL;
    utils::baseline::Equals cons(pattern);
    CHECK(!cons(line1));
    CHECK(!cons(line2));
    CHECK(cons(line3));
}

TEST_CASE("utils::Timestamp", "") {
    using value_type = utils::Timestamp;
    value_type tm;
    utils::TimePrinter printer("%m-%d-%Y %H:%M:%S");
    value_type t1(1, 3, 2018, 10, 10, 9);
    value_type t2(2, 3, 2018, 5, 21, 49);
    value_type t3(2, 5, 2018, 0, 1, 3);
    value_type t4;

    printer(t1.to_tm());
    fmt::print("t1: {}\n", printer.buffer);

    printer(t2.to_tm());
    fmt::print("t2: {}\n", printer.buffer);

    printer(t3.to_tm());
    fmt::print("t3: {}\n", printer.buffer);

    printer(t3.to_tm());
    CHECK_THAT(printer.buffer, Equals("02-05-2018 00:01:03"));
    fmt::print("t3: {}\n", printer.buffer);

    printer(t4.to_tm());
    CHECK_THAT(printer.buffer, Equals("01-01-1900 00:00:00"));

    SECTION("AllTimestamps matcher") {
        utils::AllTimestamps<value_type> cons;
        CHECK(cons(tm));
    }

    SECTION("OlderThan matcher") {
        utils::OlderThan<value_type> t2_is_older_than(t2);
        CHECK(!t2_is_older_than(t1));
        CHECK(t2_is_older_than(t3));
    }

    SECTION("NewerThan matcher") {
        utils::NewerThan<value_type> t2_is_newer_than(t2);
        CHECK(t2_is_newer_than(t1));
        CHECK(!t2_is_newer_than(t3));
    }

    SECTION("Equals matcher") {
        utils::Equals<value_type> constraint(t2);
        CHECK(!constraint(t1));
        CHECK(constraint(t2));
        CHECK(!constraint(t3));
    }

    SECTION("Between matcher") {
        utils::Between<value_type> constraint(t1, t3);
        CHECK(!constraint(t1));
        CHECK(constraint(t2));
        CHECK(!constraint(t3));
    }
}

TEST_CASE("Regular expression matcher", "utils::RegexMatcher") {
    std::string line1("This is my first line");
    std::string line2("This is my second line");
    std::string line3("");

    SECTION("Simple test") {
        utils::hyperscan::RegexMatcher matcher("my.*line");
        CHECK(matcher(line1));
        CHECK(matcher(line2));
        CHECK(!matcher(line3));
    }

    SECTION("Negative tests") {
        utils::hyperscan::RegexMatcher matcher("foo");
        CHECK(!matcher(line1));
        CHECK(!matcher(line2));
    }
}
