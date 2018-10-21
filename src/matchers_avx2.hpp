#pragma once
#include "system.hpp"
#include "string.hpp"

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
} // namespace utils
