#include "regex_matchers.hpp"
#include "fmt/format.h"
#include <string>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

TEST_CASE("utils::RegexMatcher") {
    std::string line1("This is my first line");
    std::string line2("This is my second line");
    std::string line3("");
    using Matcher = utils::hyperscan::RegexMatcher;

    SUBCASE("Simple test") {
        Matcher matcher("my.*line");
        CHECK(matcher.is_matched(line1.data(), static_cast<unsigned int>(line1.size())));
        CHECK(matcher.is_matched(line2.data(), static_cast<unsigned int>(line2.size())));
        CHECK(!matcher.is_matched(line3.data(), static_cast<unsigned int>(line3.size())));
    }

    SUBCASE("More tests") {
        Matcher matcher("foo");
        CHECK(!matcher.is_matched(line1.data(), static_cast<unsigned int>(line1.size())));
        CHECK(!matcher.is_matched(line2.data(), static_cast<unsigned int>(line2.size())));
    }

    SUBCASE("Null and zero len tests") {
        Matcher matcher("foo");
        CHECK(!matcher.is_matched(nullptr, 10));
        CHECK(!matcher.is_matched(line1.data(), 0));
    }
}

TEST_CASE("utils::RegexMatcherInv") {
    std::string line1("This is my first line");
    std::string line2("This is my second line");
    std::string line3("");
    using Matcher = utils::hyperscan::RegexMatcherInv;

    SUBCASE("Simple test") {
        Matcher matcher("my.*line");
        CHECK(!matcher.is_matched(line1.data(), static_cast<unsigned int>(line1.size())));
        CHECK(!matcher.is_matched(line2.data(), static_cast<unsigned int>(line2.size())));
        CHECK(!matcher.is_matched(line3.data(), static_cast<unsigned int>(line3.size())));
    }

    SUBCASE("More tests") {
        Matcher matcher("foo");
        CHECK(matcher.is_matched(line1.data(), static_cast<unsigned int>(line1.size())));
        CHECK(matcher.is_matched(line2.data(), static_cast<unsigned int>(line2.size())));
    }

    SUBCASE("Null and zero len tests") {
        Matcher matcher("foo");
        CHECK(!matcher.is_matched(nullptr, 10));
        CHECK(!matcher.is_matched(line1.data(), 0));
    }
}
