#pragma once

#include "fmt/format.h"
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
                    fmt::MemoryWriter writer;
                    writer << "Unable to compile pattern \"" << pattern
                           << "\": " << compile_err->message;
                    throw std::runtime_error(writer.str());
                }

                if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
                    throw std::runtime_error("Unable to allocate scratch space.");
                }
            }

            ~RegexMatcher() {
                hs_free_scratch(scratch);
                hs_free_database(database);
            }

            bool operator()(const std::string &data) {
				if (data.empty()) return false;
                char *ptr = const_cast<char *>(pattern.c_str());
                auto errcode =
                    hs_scan(database, data.data(), data.size(), 0, scratch, event_handler, ptr);
                if (errcode == HS_SUCCESS) {
                    return false;
                } else if (errcode == HS_SCAN_TERMINATED) {
                    return true;
                } else {
                    throw std::runtime_error("Unable to scan input buffer");
                }
            }

          private:
            hs_database_t *database = NULL;
            hs_scratch_t *scratch = NULL;
            std::string pattern;
        };
    } // namespace hyperscan
} // namespace utils
