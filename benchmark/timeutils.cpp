#include <benchmark/benchmark.h>
#include "matchers.hpp"
#include "timeutils.hpp"

char timestamp[] = "02-03-2018 10:12:10";

// Basic implementation using strptime.
void strftime(benchmark::State &state) {
	struct tm tm;
    for (auto _ : state) {
		strptime(timestamp, "%m-%d-%Y %H:%M:%S", &tm);
		benchmark::DoNotOptimize(mktime(&tm));
	}
}
BENCHMARK(strftime);

// Customized parser
void customized_parser(benchmark::State &state) {
	utils::timestamp::scribe parser;
    for (auto _ : state) {
		benchmark::DoNotOptimize(parser(timestamp));
	}
}
BENCHMARK(customized_parser);

// Simple parser
void simple_parser(benchmark::State &state) {
	utils::timestamp::scribe_simple parser;
    for (auto _ : state) {
		benchmark::DoNotOptimize(parser(timestamp));
	}
}
BENCHMARK(simple_parser);

// Very fast time parser
void fast_parser(benchmark::State &state) {
    for (auto _ : state) {
		benchmark::DoNotOptimize(utils::parse_timestamp<utils::Timestamp>(timestamp));
	}
}
BENCHMARK(fast_parser);

BENCHMARK_MAIN();
