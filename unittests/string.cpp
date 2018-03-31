#include "fmt/format.h"
#include <string>

#include "memchr.hpp"

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

using Catch::Matchers::Equals;

TEST_CASE("avx2_memchr", "memchr") {
    const std::string line(
        "[02/04/2018 23:42:22 job483.athenahealth.com db.db92.urgent 103213] "
        "{'LEVEL':'info','MESSAGE':'received\n','PREFIX':'WnfgrqwYFLwAAPQIAAAAWQ'}");
}
