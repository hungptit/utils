#pragma once

#include "string.hpp"
#include <cstring>
#include <string>

namespace utils {
    // String matchers we should make our mathers as fast as we can.
    class AllPatterns {
      public:
        bool operator()(const char *, const char *) { return true; }
    };

    namespace baseline {
        // Check that a searched string has a given pattern.
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
            explicit Contains(const std::string &patt) : pattern(patt) {}
            bool operator()(const std::string &line) {
                if (line.size() < pattern.size()) { return false; }
                return line.find(pattern) != std::string::npos;
            }

          private:
            const std::string pattern;
        };

        // Check that two strings are equal.
        class Equals {
          public:
            explicit Equals(const std::string &patt) : pattern(patt) {}
            bool operator()(const std::string &line) { return line == pattern; }

          private:
            const std::string pattern;
        };
    } // namespace baseline

    namespace sse2 {
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
