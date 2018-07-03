#pragma once

#include <regex>
#include <string>

namespace experiments {
    // A sample regex search using std::regex
    struct RegexMatcher {
        RegexMatcher(const std::string &patt) : search_pattern(patt) {}
        bool is_matched(const char *begin, const size_t len) {
            return std::regex_search(begin, begin + len, search_pattern);
        }
        std::regex search_pattern;
    };

    // Exact matching algorithms
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

}
