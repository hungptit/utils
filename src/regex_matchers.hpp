#pragma once

#include "hs/hs.h"
#include <string>

namespace utils {
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
                if (hs_compile(pattern.c_str(), HS_FLAG_DOTALL, HS_MODE_BLOCK, NULL, &database,
                               &compile_err) != HS_SUCCESS) {
					std::string errmsg;
                    errmsg +=  "Unable to compile pattern \"" + pattern + "\": " + compile_err->message;
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

            bool operator()(const char *ptr, const unsigned int len) {
                if (ptr == NULL) return false;
                auto errcode =
                    hs_scan(database, ptr, len , 0, scratch, event_handler, NULL);
                if (errcode == HS_SUCCESS) {
                    return false;
                } else if (errcode == HS_SCAN_TERMINATED) {
                    return true;
                } else {
                    throw std::runtime_error("Unable to scan the input buffer");
                }
            }

          private:
            hs_database_t *database = NULL;
            hs_scratch_t *scratch = NULL;
            std::string pattern;
        };
    } // namespace hyperscan
} // namespace utils
