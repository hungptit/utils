#include <array>
#include <benchmark/benchmark.h>
#include <string>

const std::array<std::string, 16> lookup_table = {
    {"PREFIX", "MESSAGE", "LEVEL", "RAW_ERROR", "RESOURCENAME", "SUBJECT", "REQUEST",
     "SENDTIME", "DESTINATION", "SERIALIZETIME", "SENDERID", "EXCHANGE"}};

bool find_slow(const std::string &pattern) {
    return std::find(lookup_table.cbegin(), lookup_table.cend(), pattern) !=
           lookup_table.cend();
}

template <int N> bool find_fast(const std::string &pattern) {
    return (lookup_table[N] == pattern) || find_fast<N - 1>(pattern);
}

template <> bool find_fast<0>(const std::string &pattern) {
    return (lookup_table[0] == pattern);
}

const std::string pattern1("RESOURCE");
const std::string pattern2("SENDERID");
const std::string pattern3("PREFIX");
const std::string pattern4("FOOOOO");
void benchmark_find_slow(benchmark::State &state) {
    for (auto _ : state) { benchmark::DoNotOptimize(find_slow(pattern1)); }
}
BENCHMARK(benchmark_find_slow);

void benchmark_find_fast(benchmark::State &state) {
    for (auto _ : state) { benchmark::DoNotOptimize(find_fast<7>(pattern1)); }
}
BENCHMARK(benchmark_find_fast);

BENCHMARK_MAIN();
