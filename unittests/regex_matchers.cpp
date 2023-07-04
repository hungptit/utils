#include "regex_matchers.hpp"
#include "fmt/format.h"
#include <string>

#define CATCH_CONFIG_MAIN
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"

using Catch::Matchers::Equals;

TEST_CASE("Regular expression matcher", "utils::RegexMatcher") {
    std::string line1("This is my first line");
    std::string line2("This is my second line");
    std::string line3("");
    using Matcher = utils::hyperscan::RegexMatcher;

    SECTION("Simple test") {
        Matcher matcher("my.*line");
        CHECK(matcher.is_matched(line1.data(), static_cast<unsigned int>(line1.size())));
        CHECK(matcher.is_matched(line2.data(), static_cast<unsigned int>(line2.size())));
        CHECK(!matcher.is_matched(line3.data(), static_cast<unsigned int>(line3.size())));
    }

    SECTION("More tests") {
        Matcher matcher("foo");
        CHECK(!matcher.is_matched(line1.data(), static_cast<unsigned int>(line1.size())));
        CHECK(!matcher.is_matched(line2.data(), static_cast<unsigned int>(line2.size())));
    }

    SECTION("Null and zero len tests") {
        Matcher matcher("foo");
        CHECK(!matcher.is_matched(nullptr, 10));
        CHECK(!matcher.is_matched(line1.data(), 0));
    }
}

TEST_CASE("Regular expression matcher - Inverse", "utils::RegexMatcherInv") {
    std::string line1("This is my first line");
    std::string line2("This is my second line");
    std::string line3("");
    using Matcher = utils::hyperscan::RegexMatcherInv;

    SECTION("Simple test") {
        Matcher matcher("my.*line");
        CHECK(!matcher.is_matched(line1.data(), static_cast<unsigned int>(line1.size())));
        CHECK(!matcher.is_matched(line2.data(), static_cast<unsigned int>(line2.size())));
        CHECK(!matcher.is_matched(line3.data(), static_cast<unsigned int>(line3.size())));
    }

    SECTION("More tests") {
        Matcher matcher("foo");
        CHECK(matcher.is_matched(line1.data(), static_cast<unsigned int>(line1.size())));
        CHECK(matcher.is_matched(line2.data(), static_cast<unsigned int>(line2.size())));
    }

    SECTION("Null and zero len tests") {
        Matcher matcher("foo");
        CHECK(!matcher.is_matched(nullptr, 10));
        CHECK(!matcher.is_matched(line1.data(), 0));
    }
}
