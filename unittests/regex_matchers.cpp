#include "fmt/format.h"
#include "regex_matchers.hpp"
#include <string>

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"
using Catch::Matchers::Equals;

TEST_CASE("Regular expression matcher", "utils::RegexMatcher") {
    std::string line1("This is my first line");
    std::string line2("This is my second line");
    std::string line3("");

    SECTION("Simple test") {
        utils::hyperscan::RegexMatcher matcher("my.*line");
        CHECK(matcher.is_matched(line1.data(), static_cast<unsigned int>(line1.size())));
        CHECK(matcher.is_matched(line2.data(), static_cast<unsigned int>(line2.size())));
        CHECK(!matcher.is_matched(line3.data(), static_cast<unsigned int>(line3.size())));
    }

    SECTION("More tests") {
        utils::hyperscan::RegexMatcher matcher("foo");
        CHECK(!matcher.is_matched(line1.data(), static_cast<unsigned int>(line1.size())));
        CHECK(!matcher.is_matched(line2.data(), static_cast<unsigned int>(line2.size())));
    }
}

TEST_CASE("Regular expression matcher - Inverse", "utils::RegexMatcherInv") {
    std::string line1("This is my first line");
    std::string line2("This is my second line");
    std::string line3("");

    SECTION("Simple test") {
        utils::hyperscan::RegexMatcherInv matcher("my.*line");
        CHECK(!matcher.is_matched(line1.data(), static_cast<unsigned int>(line1.size())));
        CHECK(!matcher.is_matched(line2.data(), static_cast<unsigned int>(line2.size())));
        CHECK(matcher.is_matched(line3.data(), static_cast<unsigned int>(line3.size())));
    }

    SECTION("More tests") {
        utils::hyperscan::RegexMatcherInv matcher("foo");
        CHECK(matcher.is_matched(line1.data(), static_cast<unsigned int>(line1.size())));
        CHECK(matcher.is_matched(line2.data(), static_cast<unsigned int>(line2.size())));
    }
}
