#pragma once

#include "hs/hs.h"
#include <regex>
#include <string>

namespace utils {
    namespace hyperscan {
        /**
         * Default implementation for the event_handler.
         */
        inline int event_handler(unsigned int, unsigned long long, unsigned long long,
                                 unsigned int, void *) {
            return HS_SCAN_TERMINATED;
        }

        /*
         * @mode: Use HS_CPU_FEATURES_AVX2 to turn on support for AVX2.
         */
        class RegexBase {
          public:
            explicit RegexBase(const std::string &patt, const int mode) {
                pattern = patt;
                hs_compile_error_t *compile_err;
                if (hs_compile(pattern.c_str(), mode, HS_MODE_BLOCK, nullptr, &database,
                               &compile_err) != HS_SUCCESS) {
                    const std::string errmsg = std::string("Unable to compile pattern \"") +
                                               pattern + "\": " + compile_err->message;
                    throw std::runtime_error(errmsg);
                }

                if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
                    throw std::runtime_error("Unable to allocate scratch space.");
                }
            }

            ~RegexBase() {
                hs_free_scratch(scratch);
                hs_free_database(database);
            }

          protected:
            hs_database_t *database = nullptr;
            hs_scratch_t *scratch = nullptr;
            std::string pattern;
        };

        template <typename Base> class RegexMatcherReg : Base {
          public:
            RegexMatcherReg(const std::string &patt,
                            const int mode = (HS_FLAG_DOTALL | HS_FLAG_SINGLEMATCH))
                : Base(patt, mode) {}

            bool operator()(const std::string &data) {
                if (data.empty()) return false;
                return is_matched(data.data(), data.size());
            }

            bool is_matched(const char *data, const size_t len) {
                return ((data != nullptr) && (len > 0)) ? is_matched_uncheck(data, len) : 0;
            }

          private:
            bool is_matched_uncheck(const char *data, const size_t len) {
                const unsigned int buflen = static_cast<unsigned int>(len);
                auto errcode = hs_scan(Base::database, data, buflen, 0, Base::scratch,
                                       event_handler, nullptr);
                if (errcode == HS_SUCCESS) {
                    return false;
                } else if (errcode == HS_SCAN_TERMINATED) {
                    return true;
                } else {
                    throw std::runtime_error("Unable to scan the input buffer");
                }
            }
        };

        using RegexMatcher = RegexMatcherReg<RegexBase>;

        template <typename Base> class RegexMatcherInverse : Base {
          public:
            RegexMatcherInverse(const std::string &patt,
                                const int mode = (HS_FLAG_DOTALL | HS_FLAG_SINGLEMATCH))
                : Base(patt, mode) {}

            bool operator()(const std::string &data) {
                if (data.empty()) return true;
                return is_matched(data.data(), data.size());
            }

            bool is_matched(const char *data, const size_t len) {
                return ((data != nullptr) && (len > 0)) ? is_matched_uncheck(data, len) : false;
            }

          private:
            bool is_matched_uncheck(const char *data, const size_t len) {
                auto errcode = hs_scan(Base::database, data, static_cast<unsigned int>(len), 0,
                                       Base::scratch, event_handler, nullptr);
                if (errcode == HS_SUCCESS) {
                    return true;
                } else if (errcode == HS_SCAN_TERMINATED) {
                    return false;
                } else {
                    throw std::runtime_error("Unable to scan the input buffer");
                }
            }
        };

        using RegexMatcherInv = RegexMatcherInverse<RegexBase>;
    } // namespace hyperscan
} // namespace utils
