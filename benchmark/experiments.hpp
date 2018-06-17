#pragma once

#include <string>
#include "string_avx2.hpp"

namespace utils {
    namespace experiments {
        struct ExactMatch {
            explicit ExactMatch(const std::string &patt) : pattern(patt) {}
            bool is_matched(const std::string &line) {
                if (line.size() < pattern.size()) { return false; }
                return line.find(pattern) != std::string::npos;
            }
            const std::string pattern;
        };

        struct ExactMatchSSE2 {
            explicit ExactMatchSSE2(const std::string &patt) : pattern(patt) {} //
            bool is_matched(const std::string &line) {
                return sse2::sse2_strstr_v2(line.data(), line.size(), pattern.data(),
                                            pattern.size()) != std::string::npos;
            }
            const std::string pattern;
        };

        struct ExactMatchAVX2 {
            explicit ExactMatchAVX2(const std::string &patt) : pattern(patt) {} //
            bool is_matched(const std::string &line) {
                return avx2::avx2_strstr_v2(line.data(), line.size(), pattern.data(),
                                            pattern.size()) != std::string::npos;
            }
            const std::string pattern;
        };

        // Check that a searched string starts with a given pattern.
        class StartsWith {
          public:
            explicit StartsWith(const std::string &patt) : pattern(patt) {}
            bool operator()(const std::string &line) {
                if (line.size() < pattern.size()) { return false; }
                return strncmp(line.data(), pattern.data(), pattern.size()) == 0;
            }

          private:
            const std::string pattern;
        };

        // Search for a sub string.
        class Contains {
          public:
            explicit Contains(const std::string &patt) : pattern(patt) {} //
            bool operator()(const std::string &line) {
                return avx2::avx2_strstr_v2(line.data(), line.size(), pattern.data(),
                                            pattern.size()) != std::string::npos;
            }

          private:
            const std::string pattern;
        };

        // Check that two strings are equal.
        class Equals {
          public:
            explicit Equals(std::string &patt) : pattern(patt) {}
            bool operator()(const std::string &line) { return line == pattern; }

          private:
            const std::string pattern;
        };
    } // namespace experiments
} // namespace utils
