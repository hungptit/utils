#pragma once

#include "string.hpp"
#include <cstring>
#include <string>

namespace utils {
    namespace sse2 {
        struct ExactMatcher {
            template <typename... Args>
            ExactMatcher(const std::string &patt, Args...) : pattern(patt) {} //
            bool is_matched(const char *begin, const size_t len) {
                return sse2::sse2_strstr_v2(begin, len, pattern.data(), pattern.size()) !=
                       std::string::npos;
            }
            const std::string pattern;
        };

        struct ExactMatcherInv {
            template <typename... Args>
            ExactMatcherInv(const std::string &patt, Args...) : pattern(patt) {} //
            bool is_matched(const char *begin, const size_t len) {
                return sse2::sse2_strstr_v2(begin, len, pattern.data(), pattern.size()) ==
                       std::string::npos;
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

        // Search for a substring.
        class Contains {
          public:
            explicit Contains(const std::string &patt) : pattern(patt) {} //
            bool operator()(const std::string &line) {
                return sse2::sse2_strstr_v2(line.data(), line.size(), pattern.data(),
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

    } // namespace sse2
} // namespace utils
