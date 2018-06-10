#pragma once

#include "hs/hs.h"
#include <string>
#include <regex>

namespace utils {
    namespace experiments {
        struct RegexMatcher {
            RegexMatcher(const std::string &patt) : search_pattern(patt) {}
            bool is_matched(const char *begin, const size_t len) {
                return std::regex_search(begin, begin + len, search_pattern);
            }
            std::regex search_pattern;
        };
    } // namespace experiments

    namespace hyperscan {
        namespace {
            // An event handle callback.
            int event_handler(unsigned int id, unsigned long long from, unsigned long long to,
                              unsigned int flags, void *ctx) {
                return HS_SCAN_TERMINATED;
            }
        } // namespace


        class RegexMatcher {
          public:
            explicit RegexMatcher(const std::string &patt) {
                pattern = patt;
                hs_compile_error_t *compile_err;
                if (hs_compile(pattern.c_str(), HS_FLAG_DOTALL, HS_MODE_BLOCK, nullptr,
                               &database, &compile_err) != HS_SUCCESS) {
                    const std::string errmsg = std::string("Unable to compile pattern \"") +
                                               pattern + "\": " + compile_err->message;
                    throw std::runtime_error(errmsg);
                }

                if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
                    throw std::runtime_error("Unable to allocate scratch space.");
                }
            }

            ~RegexMatcher() {
                hs_free_scratch(scratch);
                hs_free_database(database);
            }

            const bool operator()(const std::string &data) {
                if (data.empty()) return true;
                return is_matched(data.data(), data.size());
            }

            const bool is_matched(const char *data, const size_t len) {
                if (data == nullptr) return true;
                auto errcode = hs_scan(database, data, len, 0, scratch, event_handler, nullptr);
                if (errcode == HS_SUCCESS) {
                    return false;
                } else if (errcode == HS_SCAN_TERMINATED) {
                    return true;
                } else {
                    throw std::runtime_error("Unable to scan the input buffer");
                }
            }

          private:
            hs_database_t *database = nullptr;
            hs_scratch_t *scratch = nullptr;
            std::string pattern;
        };
    } // namespace hyperscan
} // namespace utils
