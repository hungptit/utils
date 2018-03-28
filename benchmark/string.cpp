#include "matchers.hpp"
#include <benchmark/benchmark.h>

const std::string
    data("[02/04/2018 23:42:22 job483.example.com db.db92.urgent 103212] "
         "{'LEVEL':'info','MESSAGE':'finished in 0.013\n','PREFIX':'WnfgrqwYG2AAAfC2AgBgdXg'}");
const std::string pattern("finished1234");

// Basic implementation.
void std_string_find(benchmark::State &state) {
    utils::baseline::Contains contains(pattern);
    for (auto _ : state) { benchmark::DoNotOptimize(contains(data)); }
}
// Register the function as a benchmark
BENCHMARK(std_string_find);

void sse2_string_find(benchmark::State &state) {
    utils::sse2::Contains contains(pattern);
    for (auto _ : state) { benchmark::DoNotOptimize(contains(data)); }
}
// Register the function as a benchmark
BENCHMARK(sse2_string_find);

// void avx2_string_find(benchmark::State &state) {
//     utils::avx2::Contains contains(pattern);
//     for (auto _ : state) { benchmark::DoNotOptimize(contains(data)); }
// }
// // Register the function as a benchmark
// BENCHMARK(avx2_string_find);

BENCHMARK_MAIN();
