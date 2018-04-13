#include "timeutils.hpp"
#include "matchers.hpp"
#include "timestamp.hpp"
#include <benchmark/benchmark.h>

char timestamp[] = "02-03-2018 10:12:10";

// Basic implementation using strptime.
void strftime(benchmark::State &state) {
    struct tm tm;
    for (auto _ : state) {
        strptime(timestamp, "%m-%d-%Y %H:%M:%S", &tm);
    }
}
BENCHMARK(strftime);

// Very fast time parser
void fast_parser(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(utils::parse_scribe_timestamp<utils::Timestamp>(timestamp));
    }
}
BENCHMARK(fast_parser);

BENCHMARK_MAIN();
