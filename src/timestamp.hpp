#pragma once

#include <ctime>
#include <tuple>

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
        explicit Timestamp() noexcept
            : tm_sec(0), tm_min(0), tm_hour(0), tm_mday(1), tm_mon(SHIFT_MONTH),
              tm_year(SHIFT_YEAR), tm_isdst(0) {}
        explicit Timestamp(const unsigned char mon, const unsigned char day,
                           const unsigned short year, const unsigned char hour,
                           const unsigned char minute, const unsigned char second) noexcept
            : tm_sec(second), tm_min(minute), tm_hour(hour), tm_mday(day), tm_mon(mon),
              tm_year(year), tm_isdst(0) {}

        // Convert to the tm struct. This is very useful when we want to print out the
        // timestamp.
        struct tm to_tm() const {
            struct tm tm;
            tm.tm_sec = tm_sec;
            tm.tm_min = tm_min;
            tm.tm_hour = tm_hour;
            tm.tm_mday = tm_mday;
            tm.tm_mon = tm_mon - SHIFT_MONTH;
            tm.tm_year = tm_year - SHIFT_YEAR;
            tm.tm_isdst = tm_isdst;
            return tm;
        }

        unsigned char tm_sec;
        unsigned char tm_min;
        unsigned char tm_hour;
        unsigned char tm_mday;
        unsigned char tm_mon;
        unsigned short tm_year;
        unsigned char tm_isdst;
    };

    bool operator==(const Timestamp t1, const Timestamp t2) {
        return std::tie(t1.tm_sec, t1.tm_min, t1.tm_hour, t1.tm_mday, t1.tm_mon, t1.tm_year) ==
               std::tie(t2.tm_sec, t2.tm_min, t2.tm_hour, t2.tm_mday, t2.tm_mon, t2.tm_year);
    }

    bool operator>(const Timestamp t1, const Timestamp t2) {
        return std::tie(t1.tm_year, t1.tm_mon, t1.tm_mday, t1.tm_hour, t1.tm_min, t1.tm_sec) >
               std::tie(t2.tm_year, t2.tm_mon, t2.tm_mday, t2.tm_hour, t2.tm_min, t2.tm_sec);
    }

    bool operator<(const Timestamp t1, const Timestamp t2) {
        return std::tie(t1.tm_year, t1.tm_mon, t1.tm_mday, t1.tm_hour, t1.tm_min, t1.tm_sec) <
               std::tie(t2.tm_year, t2.tm_mon, t2.tm_mday, t2.tm_hour, t2.tm_min, t2.tm_sec);
    }

    template <typename T> T parse_timestamp(const char *ptr) {
        T tm;
        tm.tm_mon = parse_digits<2>(ptr, 0);
        tm.tm_mday = parse_digits<2>(ptr + 3, 0);
        tm.tm_year = parse_digits<4>(ptr + 6, 0);
        tm.tm_hour = parse_digits<2>(ptr + 11, 0);
        tm.tm_min = parse_digits<2>(ptr + 14, 0);
        tm.tm_sec = parse_digits<2>(ptr + 17, 0);
        tm.tm_isdst = 0; // We do not care about day light saving when parsing log data.
        return tm;
    }

    // Time constraints
    template <typename T> struct AllTimestamps {
        bool operator()(const T) { return true; }
    };

    template <typename T> struct OlderThan {
        explicit OlderThan(T t) : timestamp(t) {}
        bool operator()(const T t) { return t > timestamp; }
        const T timestamp;
    };

    template <typename T> struct NewerThan {
        explicit NewerThan(const T t) : timestamp(t) {}
        bool operator()(const T t) { return t < timestamp; }
        const T timestamp;
    };

    template <typename T> struct Equals {
        explicit Equals(const T t) : timestamp(t) {}
        bool operator()(const T t) { return t == timestamp; }
        const T timestamp;
    };

    template <typename T> struct Between {
        Between(const T b, const T e) : begin(b), end(e) {}
        bool operator()(const T t) { return (t > begin) && (t < end); }
        const T begin;
        const T end;
    };
}
