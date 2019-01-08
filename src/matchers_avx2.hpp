#pragma once
#include "string.hpp"
#include "system.hpp"

namespace utils {
    namespace avx2 {
        class ExactMatcher {
          public:
            // This constructor only takes one input arguments, however, we will support
            // more that one to make it compartible to other matchers.
            template <typename... Args>
            ExactMatcher(const std::string &patt, Args...) : pattern(patt) {}

            bool is_matched(const char *begin, const size_t len) {
                return avx2::avx2_strstr_v2(begin, len, pattern.data(), pattern.size()) !=
                       std::string::npos;
            }

          private:
            std::string pattern;
        };

        // Find lines that does not match the search pattern.
        class ExactMatcherInv {
          public:
            // This constructor only takes one input arguments, however, we will support
            // more that one to make it compartible to other matchers.
            template <typename... Args>
            ExactMatcherInv(const std::string &patt, Args...) : pattern(patt) {}

            bool is_matched(const char *begin, const size_t len) {
                return avx2::avx2_strstr_v2(begin, len, pattern.data(), pattern.size()) ==
                       std::string::npos;
            }

          private:
            std::string pattern;
        };

    } // namespace avx2
} // namespace utils
