#include <benchmark/benchmark.h>
#include "utils/matchers.hpp"

std::vector<std::string> timestamps = {"2018-03-23 10:12:10"};

// Basic implementation.
void strftime(benchmark::State &state) {
	utils::baseline::Contains contains(pattern);
    for (auto _ : state) {
		benchmark::DoNotOptimize(contains(data));
	}
}
// Register the function as a benchmark
BENCHMARK(strftime);

void customized_parser(benchmark::State &state) {
	utils::sse2::Contains contains(pattern);
    for (auto _ : state) {
		benchmark::DoNotOptimize(contains(data));
	}
}
// Register the function as a benchmark
BENCHMARK(customized_parser);

BENCHMARK_MAIN();
