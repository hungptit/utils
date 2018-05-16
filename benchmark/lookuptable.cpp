#include <array>
#include <benchmark/benchmark.h>
#include <string>
#include "strcmp.hpp"

const std::array<std::string, 16> lookup_table = {
    {"PREFIX", "MESSAGE", "LEVEL", "RAW_ERROR", "RESOURCENAME", "SUBJECT", "REQUEST",
     "SENDTIME", "DESTINATION", "SERIALIZETIME", "SENDERID", "EXCHANGE"}};
const std::string lookup_string = "PREFIX,MESSAGE,LEVEL,RAW_ERROR,RESOURCENAME,SUBJECT,REQUEST,"
                                  "SENDTIME,DESTINATION,SERIALIZETIME,SENDERID,EXCHANGE";

bool is_equal(const std::string &first, const std::string &second) {
    if (first.size() != second.size()) return false;
    return utils::strncmp_avx2(first.data(), second.data(), first.size());
}

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

template <int N> bool find_fast_strcmp(const std::string &pattern) {
    return (lookup_table[N] == pattern) || find_fast_strcmp<N - 1>(pattern);
}

template <> bool find_fast_strcmp<0>(const std::string &pattern) {
    return is_equal(lookup_table[0], pattern);
}

const std::string pattern1("RESOURCE");
const std::string pattern2("SENDERID");
const std::string pattern3("PREFIX");
const std::string pattern4("FOOOOO");
const std::string pattern5("EXCHANGE");

void benchmark_find_slow(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(find_slow(pattern1));
        benchmark::DoNotOptimize(find_slow(pattern2));
        benchmark::DoNotOptimize(find_slow(pattern3));
        benchmark::DoNotOptimize(find_slow(pattern4));
        benchmark::DoNotOptimize(find_slow(pattern5));
    }
}
BENCHMARK(benchmark_find_slow);

void benchmark_find_fast(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(find_fast<7>(pattern1));
        benchmark::DoNotOptimize(find_fast<7>(pattern2));
        benchmark::DoNotOptimize(find_fast<7>(pattern3));
        benchmark::DoNotOptimize(find_fast<7>(pattern4));
        benchmark::DoNotOptimize(find_fast<7>(pattern5));
    }
}

BENCHMARK(benchmark_find_fast);

void benchmark_find_strcmp(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(find_fast_strcmp<7>(pattern1));
        benchmark::DoNotOptimize(find_fast_strcmp<7>(pattern2));
        benchmark::DoNotOptimize(find_fast_strcmp<7>(pattern3));
        benchmark::DoNotOptimize(find_fast_strcmp<7>(pattern4));
        benchmark::DoNotOptimize(find_fast_strcmp<7>(pattern5));
    }
}

BENCHMARK(benchmark_find_strcmp);


BENCHMARK_MAIN();
