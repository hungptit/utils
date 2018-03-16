#include "matchers.hpp"
#include "fmt/format.h"
#include "timeutils.hpp"
#include <cstring>
#include <string>
#include <time.h>

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

TEST_CASE("StartsWith", "StartsWith") {
    const std::string pattern("This");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");
    const char *foo = NULL;
    utils::StartsWith cons(pattern);
    CHECK(cons(line1.c_str(), line1.size()));
    CHECK(cons(line2.c_str(), line2.size()));
    CHECK(!cons(line3.c_str(), line3.size()));
}

TEST_CASE("Contains", "Contains") {
    const std::string pattern("the");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");
    const char *foo = NULL;
    utils::Contains cons(pattern);
    CHECK(cons(line1.c_str(), line1.size()));
    CHECK(cons(line2.c_str(), line2.size()));
    CHECK(!cons(line3.c_str(), line3.size()));
}

TEST_CASE("Equals", "Equals") {
    const std::string pattern("My first line!");
    const std::string line1("This is the first line!");
    const std::string line2("Thisn't the first line!");
    const std::string line3("My first line!");
    const char *foo = NULL;
    utils::Equals cons(pattern);
    CHECK(!cons(line1.c_str(), line1.size()));
    CHECK(!cons(line2.c_str(), line2.size()));
    CHECK(cons(line3.c_str(), line3.size()));
}
