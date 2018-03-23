#include "matchers.hpp"
#include "fmt/format.h"
#include "timeutils.hpp"
#include <cstring>
#include <string>
#include <time.h>

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

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
