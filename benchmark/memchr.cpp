#include "memchr.hpp"
#include <cstring>

#include <benchmark/benchmark.h>

const std::string
    data("[02/04/2018 23:42:22 job483.example.com db.db92.urgent 103212] "
         "{'LEVEL':'info','MESSAGE':'finished in 0.013\n','PREFIX':'WnfgrqwYG2AAAfC2AgBgdXg'}");
const std::string pattern("finished1234");

// memchr
void memchr(benchmark::State &state) {
    for (auto _ : state) { benchmark::DoNotOptimize(memchr(data.data(), 'W', data.size())); }
}
// Register the function as a benchmark
BENCHMARK(memchr);

// memchr
void memchr_avx2(benchmark::State &state) {
    for (auto _ : state) { benchmark::DoNotOptimize(memchr_avx2(data.data(), 'W', data.size())); }
}
// Register the function as a benchmark
BENCHMARK(memchr_avx2);

BENCHMARK_MAIN();
