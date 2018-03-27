#include <benchmark/benchmark.h>
#include "matchers.hpp"
#include "timeutils.hpp"

char timestamp[] = "02-03-2018 10:12:10";

// Basic implementation.
void strftime(benchmark::State &state) {
	struct tm tm;
    for (auto _ : state) {
		strptime(timestamp, "%m-%d-%Y %H:%M:%S", &tm);
		benchmark::DoNotOptimize(mktime(&tm));
	}
}
// Register the function as a benchmark
BENCHMARK(strftime);

void customized_parser(benchmark::State &state) {
	utils::timestamp::scribe parser;
    for (auto _ : state) {
		benchmark::DoNotOptimize(parser(timestamp));
	}
}
// Register the function as a benchmark
BENCHMARK(customized_parser);

void fast_parser(benchmark::State &state) {
    for (auto _ : state) {
		benchmark::DoNotOptimize(utils::parse_timestamp(timestamp));
	}
}
// Register the function as a benchmark
BENCHMARK(fast_parser);

BENCHMARK_MAIN();
