#pragma once
#include "string_avx2.hpp"
#include "system.hpp"

namespace utils {
    class ExactMatchAVX2 {
      public:
        explicit ExactMatchAVX2(const std::string &patt) {
            pattern = static_cast<char *>(malloc(patt.size()));
            buflen = patt.size();
            memcpy(pattern, patt.data(), buflen);
        }

        ~ExactMatchAVX2() { free(pattern); }

        bool is_matched(const char *begin, const size_t len) {
            return avx2::avx2_strstr_v2(begin, len, pattern, buflen) != std::string::npos;
        }

      private:
        char *pattern;
        size_t buflen;
    };

    namespace experiments {
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
