#pragma once

#include <cstring>
#include <string>

namespace utils {
    // String matchers we should make our mathers as fast as we can.
    class AllPatterns {
      public:
        bool operator()(const char *, const char *) { return true; }
    };

	// Check that a searched string has a given pattern.
    struct StartsWith {
      public:
        template <typename String>
        explicit StartsWith(String &&patt) : pattern(std::forward<String>(patt)) {
            begin = pattern.c_str();
            end = begin + pattern.size();
        }
        bool operator()(const char *b, size_t len) {
            if (len < pattern.size()) {
                return 0;
            }
            return strncmp(b, begin, pattern.size()) == 0;
        }

      private:
        const std::string pattern;
        const char *begin;
        const char *end;
    };

	// Search for a sub string.
    struct Contains {
      public:
        template <typename String>
        explicit Contains(String &&patt) : pattern(std::forward<String>(patt)) {
            begin = pattern.c_str();
            end = begin + pattern.size();
        }
        bool operator()(const char *beg, size_t len) {
            if (len < pattern.size()) {
                return 0;
            }
            return strstr(beg, begin) != NULL;
        }

      private:
        const std::string pattern;
        const char *begin;
        const char *end;
    };

	// Check that two strings are equal.
    struct Equals {
      public:
        template <typename String>
        explicit Equals(String &&patt) : pattern(std::forward<String>(patt)) {
            begin = pattern.c_str();
            end = begin + pattern.size();
        }
        bool operator()(const char *beg, size_t len) {
            if (len != pattern.size()) {
                return 0;
            }
            return strncmp(beg, begin, len) == 0;
        }

      private:
        const std::string pattern;
        const char *begin;
        const char *end;
    };

} // namespace utils
