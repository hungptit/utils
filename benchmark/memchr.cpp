#include "memchr.hpp"
#include <benchmark/benchmark.h>
#include <cstring>

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

#ifdef USE_AVX2
// memchr_avx2 short
void memchr_avx2_short(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(utils::avx2::memchr(data.data(), ch1, data.size()));
    }
}
BENCHMARK(memchr_avx2_short);

// memchr_avx2 long
void memchr_avx2_long(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(utils::avx2::memchr(data.data(), ch2, data.size()));
    }
}
BENCHMARK(memchr_avx2_long);
#endif

BENCHMARK_MAIN();
