#include "fmt/format.h"
#include <string>

#include "fmt/format.h"
#include "memchr.hpp"
#include "strcmp.hpp"

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

using Catch::Matchers::Equals;

TEST_CASE("avx2_memchr", "memchr") {
    const std::string line(
        "[02/04/2018 23:42:22 job483.athenahealth.com db.db92.urgent 103213] "
        "{'LEVEL':'info','MESSAGE':'received\n','PREFIX':'WnfgrqwYFLwAAPQIAAAAWQ'}");

	// Simple tests for memchr
	CHECK(memchr(line.data(), '*', line.size()) == NULL);
	CHECK(memchr(line.data(), '2', line.size()) != NULL);
	CHECK(static_cast<const char*>(memchr(line.data(), '2', line.size())) == (line.data() + 2));
	CHECK(static_cast<const char*>(memchr(line.data(), ' ', line.size())) == (line.data() + 11));
	CHECK(static_cast<const char*>(memchr(line.data(), 'W', line.size())) == (line.data() + 116));

	// Simple tests for memchr_avx2
	CHECK(memchr_avx2(line.data(), '*', line.size()) == NULL);
	CHECK(memchr_avx2(line.data(), '2', line.size()) != NULL);
	CHECK(static_cast<const char*>(memchr_avx2(line.data(), '2', line.size())) == (line.data() + 2));
	CHECK(static_cast<const char*>(memchr_avx2(line.data(), ' ', line.size())) == (line.data() + 11));
	CHECK(static_cast<const char*>(memchr_avx2(line.data(), 'W', line.size())) == (line.data() + 116));

	// Simple manual test
	auto ptr = memchr_avx2(line.data(), ']', line.size());
	fmt::print("Data: {}\n", ptr + 1);
}


TEST_CASE("strncmp", "SIMD implementations of strncmp") {
	std::string line1("Hello");
	std::string line2("Helle");

	CHECK(strncmp(line1.data(), line2.data(), line1.size()) != 0);
	CHECK(strncmp(line1.data(), line1.data(), line1.size()) == 0);

	CHECK(!utils::strncmp_sse2(line1.data(), line2.data(), line1.size()));
	CHECK(utils::strncmp_sse2(line1.data(), line1.data(), line1.size()));
	
	CHECK(!utils::strncmp_avx2(line1.data(), line2.data(), line1.size()));
	CHECK(utils::strncmp_avx2(line1.data(), line1.data(), line1.size()));
}
