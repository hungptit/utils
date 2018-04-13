#include "timeutils.hpp"
#include "matchers.hpp"
#include "timestamp.hpp"
#include "timestamp_experiments.hpp"
#include <benchmark/benchmark.h>

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
    utils::experiments::scribe parser;
    for (auto _ : state) { benchmark::DoNotOptimize(parser(timestamp)); }
}
BENCHMARK(customized_parser);

// Simple parser
void simple_parser(benchmark::State &state) {
    utils::experiments::scribe_simple parser;
    for (auto _ : state) { benchmark::DoNotOptimize(parser(timestamp)); }
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
