#pragma once

#include <ctime>
#include <string>
#include <functional>

namespace utils {
    namespace {
        static constexpr char ZERO = '0';
        static constexpr int SHIFT_2 = ZERO * 11;
        static constexpr int SHIFT_4 = ZERO * 1111;
        static constexpr int SHIFT_YEAR = 1900;
        static constexpr int SHIFT_MONTH = 1;
    }

    template <unsigned int N> int parse_digits(const char *ptr, const int initval) {
        const int val = initval * 10 + ptr[0] - ZERO;
        return parse_digits<N - 1>(++ptr, val);
    }

    template <> int parse_digits<1>(const char *ptr, const int initval) {
        return initval * 10 + ptr[0] - ZERO;
    }

    // A simple data structure to hold time information. This data structure
    // must be similar to struct tm so that we can reuse our parsing code.
    struct Timestamp {
        unsigned char tm_sec;
        unsigned char tm_min;
        unsigned char tm_hour;
        unsigned char tm_mday;
        unsigned char tm_mon;
        unsigned short tm_year;
        unsigned char tm_isdst;
    };

    // TODO: We need to have comparators and hash functor for Timestamp.
    template <typename T> T parse_timestamp(const char *ptr) {
        T tm;
        tm.tm_mon = parse_digits<2>(ptr, 0);
        tm.tm_mday = parse_digits<2>(ptr + 3, 0);
        tm.tm_year = parse_digits<4>(ptr + 6, 0);
        tm.tm_hour = parse_digits<2>(ptr + 11, 0);
        tm.tm_min = parse_digits<2>(ptr + 14, 0);
        tm.tm_sec = parse_digits<2>(ptr + 17, 0);
        tm.tm_isdst = 0; // Do not care about day light saving.
        return tm;
    }

    // A simple parser for timestamp.
    namespace timestamp {
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

        // TODO: Convert these structs into template so we can reuse logics.
        struct AllTimestamps {
            bool operator()(const std::time_t t) { return true; }
        };

        struct OlderThan {
            explicit OlderThan(const std::time_t t) : timestamp(t) {}
            bool operator()(const std::time_t t) { return t > timestamp; }
            const std::time_t timestamp;
        };

        struct NewerThan {
            explicit NewerThan(const std::time_t t) : timestamp(t) {}
            bool operator()(const std::time_t t) { return t < timestamp; }
            const std::time_t timestamp;
        };

        struct Equals {
            explicit Equals(const std::time_t t) : timestamp(t) {}
            bool operator()(const std::time_t t) { return t == timestamp; }
            const std::time_t timestamp;
        };

        struct Between {
            Between(const std::time_t b, const std::time_t e) : begin(b), end(e) {}

            bool operator()(const std::time_t t) { return (t > begin) && (t < end); }

            const std::time_t begin;
            const std::time_t end;
        };
    }

    class TimeParser {
      public:
        TimeParser(const std::string &fmt) : format(fmt) {}
        std::time_t operator()(const char *timestr) {
            strptime(timestr, format.c_str(), &timestamp);
            timestamp.tm_isdst = 0; // TODO: Disable day light time saving for now.
            return mktime(&timestamp);
        }

      private:
        std::string format;
        struct tm timestamp;
    };

    // A simple time printer class.
    struct TimePrinter {
        TimePrinter(const std::string &fmt) : format(fmt) {}

        const char *operator()(const std::time_t t) {
            struct tm *timestamp = localtime(&t);
            strftime(buffer, buffer_size, format.c_str(), timestamp);
            return buffer;
        }

        const char *operator()(const struct tm *timestamp) {
            strftime(buffer, buffer_size, format.c_str(), timestamp);
            return buffer;
        }

        std::string format;
        static constexpr size_t buffer_size = 255;
        char buffer[buffer_size];
    };
} // namespace utils
