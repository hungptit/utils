#include "memchr.hpp"
#include <cstring>
#include <benchmark/benchmark.h>

const std::string
    data("[02/04/2018 23:42:22 job483.example.com db.db92.urgent 103212] "
         "{'LEVEL':'info','MESSAGE':'finished in 0.013\n','PREFIX':'WnfgrqwYG2AAAfC2AgBgdXg'}");

const char ch1('2');
const char ch2('W');

// memchr short
void memchr_short(benchmark::State &state) {
    for (auto _ : state) { benchmark::DoNotOptimize(memchr(data.data(), ch1, data.size())); }
}
BENCHMARK(memchr_short);

// memchr long
void memchr_long(benchmark::State &state) {
    for (auto _ : state) { benchmark::DoNotOptimize(memchr(data.data(), ch2, data.size())); }
}
BENCHMARK(memchr_long);

// memchr_avx2 short
void memchr_avx2_short(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(memchr_avx2(data.data(), ch1, data.size()));
    }
}
BENCHMARK(memchr_avx2_short);

// memchr_avx2 long
void memchr_avx2_long(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(memchr_avx2(data.data(), ch2, data.size()));
    }
}
BENCHMARK(memchr_avx2_long);

// memchr_avx2 short
void memchr_avx2_v2_short(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(memchr_avx2_v2(data.data(), ch1, data.size()));
    }
}
BENCHMARK(memchr_avx2_v2_short);

// memchr_avx2 long
void memchr_avx2_v2_long(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(memchr_avx2_v2(data.data(), ch2, data.size()));
    }
}
BENCHMARK(memchr_avx2_v2_long);

BENCHMARK_MAIN();
