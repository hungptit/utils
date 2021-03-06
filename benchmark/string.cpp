#include "matchers.hpp"
#include "strcmp.hpp"
#include <array>
#include <benchmark/benchmark.h>

#include "experiments.hpp"

const std::string
    data("[02/04/2018 23:42:22 job483.example.com db.db92.urgent 103212] "
         "{'LEVEL':'info','MESSAGE':'finished in 0.013\n','PREFIX':'WnfgrqwYG2AAAfC2AgBgdXg'}");

const std::string data1(
    "[02/04/2018 23:42:22 job483.example.com db.db92.urgent 103212] "
    "{'LEVEL':'info','MESSAGE':'finished in 0.013\n','PREFIX':'WnfgrqwYG2AAAfC2AgBgdXg'}");

const std::string data2(
    "[02/04/2018 23:42:22 job483.example.com db.db92.urgent 103212] "
    "{'LEVEL':'info','MESSAGE':'finished in 0.013\n','PREFIX':'WnfgrqwYG2AAAfC2AgBgdXg'}");

const std::string pattern("finished1234");

// Basic implementation.
void string_find_std(benchmark::State &state) {
    utils::experiments::ExactMatch contains(pattern);
    for (auto _ : state) { benchmark::DoNotOptimize(contains.is_matched(data)); }
}
// Register the function as a benchmark
BENCHMARK(string_find_std);

void string_find_sse2(benchmark::State &state) {
    utils::sse2::Contains contains(pattern);
    for (auto _ : state) { benchmark::DoNotOptimize(contains(data)); }
}
// Register the function as a benchmark
BENCHMARK(string_find_sse2);

// ==
void string_equal(benchmark::State &state) {
    for (auto _ : state) { benchmark::DoNotOptimize(data1 == data2); }
}
BENCHMARK(string_equal);

// strncmp
void string_strncmp(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(strncmp(data1.data(), data2.data(), data1.size()));
    }
}

BENCHMARK(string_strncmp);

void string_memcmp(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(memcmp(data1.data(), data2.data(), data1.size()));
    }
}

BENCHMARK(string_memcmp);

// SSE2 version of strncmp
void strcmp_sse2(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(
            utils::sse2::strncmp(data1.data(), data2.data(), data1.size()));
    }
}
BENCHMARK(strcmp_sse2);

// AVX2 implementation of strncmp
#ifdef USE_AVX2
void string_find_avx2(benchmark::State &state) {
    utils::avx2::ExactMatcher contains(pattern);
    for (auto _ : state) {
        benchmark::DoNotOptimize(contains.is_matched(data.data(), data.size()));
    }
}
BENCHMARK(string_find_avx2);

void strcmp_avx2(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(
            utils::avx2::strncmp(data1.data(), data2.data(), data1.size()));
    }
}
BENCHMARK(strcmp_avx2);
#endif

BENCHMARK_MAIN();
