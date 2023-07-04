#include "catch2/matchers/catch_matchers_string.hpp"
#include "fmt/format.h"
#include <string>

#include "timestamp.hpp"
#include "timeutils.hpp"
#include <time.h>

#define CATCH_CONFIG_MAIN
#include "catch2/catch_test_macros.hpp"

using Catch::Matchers::Equals;

void print(const utils::Timestamp t, const std::string &msg) {
    utils::TimePrinter printer("%Y-%m-%d %H:%M:%S");
    printer(t.to_tm());
    fmt::print("{0}:{1}:{2} {3}:{4}:{5}\n", t.tm_year(), t.tm_mon(), t.tm_mday(), t.tm_hour(),
               t.tm_min(), t.tm_sec());
    fmt::print("{0}: {1}\n", msg, printer.buffer);
}

TEST_CASE("Constructor", "utils::Timestamp") {
    utils::Timestamp tmin = utils::MIN_TIME;
    utils::Timestamp tmax = utils::MAX_TIME;
    utils::Timestamp t1(2018, 3, 7, 1, 12, 34);
    utils::Timestamp t2(2005, 1, 10, 12, 57, 4);
    utils::Timestamp t3(2017, 9, 12, 23, 1, 56);
    print(tmin, "tmin");
    print(tmax, "tmax");
    print(t1, "t1");
    print(t2, "t2");
    print(t3, "t3");

    utils::TimePrinter printer("%Y-%m-%d %H:%M:%S");
    CHECK_THAT(printer(tmin.to_tm()), Equals("1900-01-01 00:00:00"));
    CHECK_THAT(printer(tmax.to_tm()), Equals("2100-01-01 00:00:00"));
    CHECK_THAT(printer(t1.to_tm()), Equals("2018-03-07 01:12:34"));
    CHECK_THAT(printer(t2.to_tm()), Equals("2005-01-10 12:57:04"));
    CHECK_THAT(printer(t3.to_tm()), Equals("2017-09-12 23:01:56"));
    CHECK(t1 > tmin);
    CHECK(t2 > tmin);
    CHECK(t3 > tmin);
    CHECK(t1 < tmax);
    CHECK(t2 < tmax);
    CHECK(t3 < tmax);
    CHECK(t1 > t2);
    CHECK(t1 > t3);
    CHECK(t3 < t1);
    CHECK(t1 != t2);
    CHECK(t1 == t1);
}
