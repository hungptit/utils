#include "matchers.hpp"
#include "strcmp.hpp"
#include <benchmark/benchmark.h>

const std::string
    data("[02/04/2018 23:42:22 job483.example.com db.db92.urgent 103212] "
         "{'LEVEL':'info','MESSAGE':'finished in 0.013\n','PREFIX':'WnfgrqwYG2AAAfC2AgBgdXg'}");

const std::string
    data1("[02/04/2018 23:42:22 job483.example.com db.db92.urgent 103212] "
         "{'LEVEL':'info','MESSAGE':'finished in 0.013\n','PREFIX':'WnfgrqwYG2AAAfC2AgBgdXg'}");

const std::string
    data2("[02/04/2018 23:42:22 job483.example.com db.db92.urgent 103212] "
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

#include "matchers_avx2.hpp"
void avx2_string_find(benchmark::State &state) {
    utils::avx2::Contains contains(pattern);
    for (auto _ : state) { benchmark::DoNotOptimize(contains(data)); }
}
BENCHMARK(avx2_string_find);

// ==
void string_equal(benchmark::State &state) {
    utils::avx2::Contains contains(pattern);
    for (auto _ : state) { benchmark::DoNotOptimize(data1 == data2); }
}
BENCHMARK(string_equal);

// strncmp
void string_strncmp(benchmark::State &state) {
    utils::avx2::Contains contains(pattern);
    for (auto _ : state) { benchmark::DoNotOptimize(strncmp(data1.data(), data2.data(), data1.size())); }
}

BENCHMARK(string_strncmp);

void string_memcmp(benchmark::State &state) {
    utils::avx2::Contains contains(pattern);
    for (auto _ : state) { benchmark::DoNotOptimize(memcmp(data1.data(), data2.data(), data1.size())); }
}

BENCHMARK(string_memcmp);


// SSE2 version of strncmp
void strcmp_sse2(benchmark::State &state) {
    utils::avx2::Contains contains(pattern);
    for (auto _ : state) { benchmark::DoNotOptimize(utils::strncmp_sse2(data1.data(), data2.data(), data1.size())); }
}
BENCHMARK(strcmp_sse2);

// avx2 version of strncmp
void strcmp_avx2(benchmark::State &state) {
    utils::avx2::Contains contains(pattern);
    for (auto _ : state) { benchmark::DoNotOptimize(utils::strncmp_avx2(data1.data(), data2.data(), data1.size())); }
}
BENCHMARK(strcmp_avx2);

BENCHMARK_MAIN();
