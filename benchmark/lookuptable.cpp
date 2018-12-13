#include "string.hpp"
#include "tsl/bhopscotch_set.h"
#include "tsl/hopscotch_set.h"
#include <array>
#include <benchmark/benchmark.h>
#include <set>
#include <string>
#include <unordered_set>

const std::array<std::string, 16> lookup_table = {
    {"PREFIX", "MESSAGE", "LEVEL", "RAW_ERROR", "RESOURCENAME", "SUBJECT", "REQUEST",
     "SENDTIME", "DESTINATION", "SERIALIZETIME", "SENDERID", "EXCHANGE"}};
const std::set<std::string> set_lookup_table(lookup_table.cbegin(), lookup_table.cend());
const std::unordered_set<std::string> hash_lookup_table(lookup_table.cbegin(),
                                                        lookup_table.cend());
const tsl::hopscotch_set<std::string> hopscotch_set_table(lookup_table.cbegin(),
                                                          lookup_table.cend());
const tsl::bhopscotch_set<std::string> bhopscotch_set_table(lookup_table.cbegin(),
                                                            lookup_table.cend());
const tsl::hopscotch_set<std::string, std::hash<std::string>, std::equal_to<std::string>,
                         std::allocator<std::string>, 24, true>
    hopscotch_set_table_new(lookup_table.cbegin(), lookup_table.cend());

bool is_equal(const std::string &first, const std::string &second) {
    return (first.size() != second.size()) &&
           strncmp(first.data(), second.data(), first.size());
}

namespace {
    struct Baseline {
        bool operator()(const std::string &pattern) {
            for (auto const &item : lookup_table) {
                if (item == pattern) return true;
            }
            return false;
        }
        const std::array<std::string, 16> lookup_table = {
            {"PREFIX", "MESSAGE", "LEVEL", "RAW_ERROR", "RESOURCENAME", "SUBJECT", "REQUEST",
             "SENDTIME", "DESTINATION", "SERIALIZETIME", "SENDERID", "EXCHANGE", "FOO1", "FOO2",
             "FOO3", "FOO4"}};
    };

    struct StdFind {
        bool operator()(const std::string &pattern) {
            return std::find(lookup_table.cbegin(), lookup_table.cend(), pattern) !=
                   lookup_table.cend();
        }
        const std::array<std::string, 16> lookup_table = {
            {"PREFIX", "MESSAGE", "LEVEL", "RAW_ERROR", "RESOURCENAME", "SUBJECT", "REQUEST",
             "SENDTIME", "DESTINATION", "SERIALIZETIME", "SENDERID", "EXCHANGE", "FOO1", "FOO2",
             "FOO3", "FOO4"}};
    };

    template <int N> bool find_fast(const std::string &pattern) {
        return (lookup_table[N] == pattern) || find_fast<N - 1>(pattern);
    }

    template <> bool find_fast<0>(const std::string &pattern) {
        return (lookup_table[0] == pattern);
    }

    struct RecursiveTemplate {
        bool operator()(const std::string &pattern) { return find_fast<15>(pattern); }
        const std::array<std::string, 16> lookup_table = {
            {"PREFIX", "MESSAGE", "LEVEL", "RAW_ERROR", "RESOURCENAME", "SUBJECT", "REQUEST",
             "SENDTIME", "DESTINATION", "SERIALIZETIME", "SENDERID", "EXCHANGE", "FOO1", "FOO2",
             "FOO3", "FOO4"}};
    };

    template <int N> bool find_fast_strcmp(const std::string &pattern) {
        return (lookup_table[N] == pattern) || find_fast_strcmp<N - 1>(pattern);
    }

    template <> bool find_fast_strcmp<0>(const std::string &pattern) {
        return is_equal(lookup_table[0], pattern);
    }

    template <int N> struct RecursiveTemplateStrcmp {
        bool operator()(const std::string &pattern) { return find_fast_strcmp<N - 1>(pattern); }
        const std::array<std::string, N> lookup_table = {
            {"PREFIX", "MESSAGE", "LEVEL", "RAW_ERROR", "RESOURCENAME", "SUBJECT", "REQUEST",
             "SENDTIME", "DESTINATION", "SERIALIZETIME", "SENDERID", "EXCHANGE", "FOO1", "FOO2",
             "FOO3", "FOO4"}};
    };

} // namespace

const std::string pattern1("RESOURCE");
const std::string pattern2("SENDERID");
const std::string pattern3("PREFIX");
const std::string pattern4("FOOOOO");
const std::string pattern5("EXCHANGE");

void benchmark_baseline(benchmark::State &state) {
    Baseline isok;
    for (auto _ : state) {
        benchmark::DoNotOptimize(isok(pattern1));
        benchmark::DoNotOptimize(isok(pattern2));
        benchmark::DoNotOptimize(isok(pattern3));
        benchmark::DoNotOptimize(isok(pattern4));
        benchmark::DoNotOptimize(isok(pattern5));
    }
}
BENCHMARK(benchmark_baseline);

void benchmark_find_std(benchmark::State &state) {
    StdFind isok;
    for (auto _ : state) {
        benchmark::DoNotOptimize(isok(pattern1));
        benchmark::DoNotOptimize(isok(pattern2));
        benchmark::DoNotOptimize(isok(pattern3));
        benchmark::DoNotOptimize(isok(pattern4));
        benchmark::DoNotOptimize(isok(pattern5));
    }
}
BENCHMARK(benchmark_find_std);

void benchmark_find_recursive_template(benchmark::State &state) {
    RecursiveTemplate isok;
    for (auto _ : state) {
        benchmark::DoNotOptimize(isok(pattern1));
        benchmark::DoNotOptimize(isok(pattern2));
        benchmark::DoNotOptimize(isok(pattern3));
        benchmark::DoNotOptimize(isok(pattern4));
        benchmark::DoNotOptimize(isok(pattern5));
    }
}
BENCHMARK(benchmark_find_recursive_template);

void benchmark_find_strcmp(benchmark::State &state) {
    RecursiveTemplateStrcmp<16> isok;
    for (auto _ : state) {
        benchmark::DoNotOptimize(isok(pattern1));
        benchmark::DoNotOptimize(isok(pattern2));
        benchmark::DoNotOptimize(isok(pattern3));
        benchmark::DoNotOptimize(isok(pattern4));
        benchmark::DoNotOptimize(isok(pattern5));
    }
}
BENCHMARK(benchmark_find_strcmp);

void benchmark_find_set(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(set_lookup_table.find(pattern1) != set_lookup_table.end());
        benchmark::DoNotOptimize(set_lookup_table.find(pattern2) != set_lookup_table.end());
        benchmark::DoNotOptimize(set_lookup_table.find(pattern3) != set_lookup_table.end());
        benchmark::DoNotOptimize(set_lookup_table.find(pattern4) != set_lookup_table.end());
        benchmark::DoNotOptimize(set_lookup_table.find(pattern5) != set_lookup_table.end());
    }
}
BENCHMARK(benchmark_find_set);

void benchmark_find_unordered_set(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(hash_lookup_table.find(pattern1) != hash_lookup_table.end());
        benchmark::DoNotOptimize(hash_lookup_table.find(pattern2) != hash_lookup_table.end());
        benchmark::DoNotOptimize(hash_lookup_table.find(pattern3) != hash_lookup_table.end());
        benchmark::DoNotOptimize(hash_lookup_table.find(pattern4) != hash_lookup_table.end());
        benchmark::DoNotOptimize(hash_lookup_table.find(pattern5) != hash_lookup_table.end());
    }
}
BENCHMARK(benchmark_find_unordered_set);

void benchmark_hopscotch_set(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(hopscotch_set_table.find(pattern1) !=
                                 hopscotch_set_table.end());
        benchmark::DoNotOptimize(hopscotch_set_table.find(pattern2) !=
                                 hopscotch_set_table.end());
        benchmark::DoNotOptimize(hopscotch_set_table.find(pattern3) !=
                                 hopscotch_set_table.end());
        benchmark::DoNotOptimize(hopscotch_set_table.find(pattern4) !=
                                 hopscotch_set_table.end());
        benchmark::DoNotOptimize(hopscotch_set_table.find(pattern5) !=
                                 hopscotch_set_table.end());
    }
}
BENCHMARK(benchmark_hopscotch_set);

void benchmark_bhopscotch_set(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(bhopscotch_set_table.find(pattern1) !=
                                 bhopscotch_set_table.end());
        benchmark::DoNotOptimize(bhopscotch_set_table.find(pattern2) !=
                                 bhopscotch_set_table.end());
        benchmark::DoNotOptimize(bhopscotch_set_table.find(pattern3) !=
                                 bhopscotch_set_table.end());
        benchmark::DoNotOptimize(bhopscotch_set_table.find(pattern4) !=
                                 bhopscotch_set_table.end());
        benchmark::DoNotOptimize(bhopscotch_set_table.find(pattern5) !=
                                 bhopscotch_set_table.end());
    }
}

BENCHMARK(benchmark_bhopscotch_set);

void benchmark_hopscotch_set_store_hash_code(benchmark::State &state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(hopscotch_set_table_new.find(pattern1) !=
                                 hopscotch_set_table_new.end());
        benchmark::DoNotOptimize(hopscotch_set_table_new.find(pattern2) !=
                                 hopscotch_set_table_new.end());
        benchmark::DoNotOptimize(hopscotch_set_table_new.find(pattern3) !=
                                 hopscotch_set_table_new.end());
        benchmark::DoNotOptimize(hopscotch_set_table_new.find(pattern4) !=
                                 hopscotch_set_table_new.end());
        benchmark::DoNotOptimize(hopscotch_set_table_new.find(pattern5) !=
                                 hopscotch_set_table_new.end());
    }
}

BENCHMARK(benchmark_hopscotch_set_store_hash_code);

BENCHMARK_MAIN();
