#pragma once

#include "timestamp.hpp"
#include <functional>

namespace utils {
    namespace experiments {
        // Scribe log message has this format "02/04/2018 23:42:22".
        // This parse doesn't validate seperator characters.
        struct scribe {
            using value_type = struct tm;
            auto operator()(const char *ptr) {
                tm.tm_mon = parse_digits<2>(ptr, 0) - SHIFT_MONTH;
                tm.tm_mday = parse_digits<2>(ptr + 3, 0);
                tm.tm_year = parse_digits<4>(ptr + 6, 0) - SHIFT_YEAR;
                tm.tm_hour = parse_digits<2>(ptr + 11, 0);
                tm.tm_min = parse_digits<2>(ptr + 14, 0);
                tm.tm_sec = parse_digits<2>(ptr + 17, 0);
                tm.tm_isdst = 0; // Do not care about day light saving.
                return mktime(&tm);
            }
            value_type tm;
        };

        struct scribe_simple {
            using value_type = struct tm;
            auto operator()(const char *ptr) {
                tm.tm_mon = parse_digits<2>(ptr, 0) - SHIFT_MONTH;
                tm.tm_mday = parse_digits<2>(ptr + 3, 0);
                tm.tm_year = parse_digits<4>(ptr + 6, 0) - SHIFT_YEAR;
                tm.tm_hour = parse_digits<2>(ptr + 11, 0);
                tm.tm_min = parse_digits<2>(ptr + 14, 0);
                tm.tm_sec = parse_digits<2>(ptr + 17, 0);
                tm.tm_isdst = 0; // Do not care about day light saving.
                return std::ref(tm);
            }
            value_type tm;
        };

        struct rabbitmq {};
    }
}
