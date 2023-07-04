#include "fmt/format.h"
#include "matchers.hpp"
#include "regex_matchers.hpp"
#include <cstring>
#include <string>

#define CATCH_CONFIG_MAIN
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"

using Catch::Matchers::Equals;

TEST_CASE("ExactMatcher for SSE2") {
    const std::string pattern("This");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");

    SECTION("Regular matcher") {
        using Matcher = utils::sse2::ExactMatcher;
        Matcher matcher(pattern);
        CHECK(matcher.is_matched(line1.data(), line1.size()));
        CHECK(matcher.is_matched(line2.data(), line2.size()));
        CHECK(!matcher.is_matched(line3.data(), line3.size()));
    }

    SECTION("Inverse matcher") {
        using Matcher = utils::sse2::ExactMatcherInv;
        Matcher matcher(pattern);
        CHECK(!matcher.is_matched(line1.data(), line1.size()));
        CHECK(!matcher.is_matched(line2.data(), line2.size()));
        CHECK(matcher.is_matched(line3.data(), line3.size()));
    }
}

#ifdef USE_AVX2
TEST_CASE("ExactMatcher for AVX2") {
    const std::string pattern("This");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");

    SECTION("Regular matcher") {
        using Matcher = utils::sse2::ExactMatcher;
        Matcher matcher(pattern);
        CHECK(matcher.is_matched(line1.data(), line1.size()));
        CHECK(matcher.is_matched(line2.data(), line2.size()));
        CHECK(!matcher.is_matched(line3.data(), line3.size()));
    }

    SECTION("Inverse matcher") {
        using Matcher = utils::sse2::ExactMatcherInv;
        Matcher matcher(pattern);
        CHECK(!matcher.is_matched(line1.data(), line1.size()));
        CHECK(!matcher.is_matched(line2.data(), line2.size()));
        CHECK(matcher.is_matched(line3.data(), line3.size()));
    }
}
#endif

TEST_CASE("ExactMatcher") {
    const std::string pattern("This");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");

    SECTION("Regular matcher") {
        using Matcher = utils::ExactMatcher;
        Matcher matcher(pattern);
        CHECK(matcher.is_matched(line1.data(), line1.size()));
        CHECK(matcher.is_matched(line2.data(), line2.size()));
        CHECK(!matcher.is_matched(line3.data(), line3.size()));
        CHECK(!matcher.is_matched(nullptr, 1));
        CHECK(!matcher.is_matched(line3.data(), 0));
    }

    SECTION("Inverse matcher") {
        using Matcher = utils::ExactMatcherInv;
        Matcher matcher(pattern);
        CHECK(!matcher.is_matched(line1.data(), line1.size()));
        CHECK(!matcher.is_matched(line2.data(), line2.size()));
        CHECK(matcher.is_matched(line3.data(), line3.size()));
        CHECK(!matcher.is_matched(nullptr, 1));
        CHECK(!matcher.is_matched(line3.data(), 0));
    }
}
