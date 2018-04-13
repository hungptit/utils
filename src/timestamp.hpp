#pragma once

#include <ctime>
#include <tuple>

namespace utils {
    static constexpr char ZERO = '0';
    static constexpr int SHIFT_2 = ZERO * 11;
    static constexpr int SHIFT_4 = ZERO * 1111;
    static constexpr int SHIFT_YEAR = 1900;
    static constexpr int SHIFT_MONTH = 1;

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
            : tm_year(SHIFT_YEAR), tm_mon(SHIFT_MONTH), tm_mday(1), tm_hour(0), tm_min(0),
              tm_sec(0), tm_isdst(0) {
            compute_epoch();
        }

        explicit Timestamp(const unsigned char mon, const unsigned char day,
                           const unsigned short year, const unsigned char hour,
                           const unsigned char minute, const unsigned char second) noexcept
            : tm_year(year), tm_mon(mon), tm_mday(day), tm_hour(hour), tm_min(minute),
              tm_sec(second), tm_isdst(0) {
            compute_epoch();
        }

        template <typename T>
        Timestamp(T &&t)
            : tm_year(t.tm_year), tm_mon(t.tm_mon), tm_mday(t.tm_mday), tm_hour(t.tm_hour),
              tm_min(t.tm_min), tm_sec(t.tm_sec), tm_isdst(t.tm_isdst) {
            compute_epoch();
        }

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

        unsigned short tm_year;
        unsigned char tm_mon;
        unsigned char tm_mday;
        unsigned char tm_hour;
        unsigned char tm_min;
        unsigned char tm_sec;
        unsigned char tm_isdst;

        void compute_epoch() {
            _value =
                (static_cast<int64_t>(tm_year) << 48) + (static_cast<int64_t>(tm_mon) << 40) +
                (static_cast<int64_t>(tm_mday) << 32) + (static_cast<int64_t>(tm_hour) << 24) +
                (static_cast<int64_t>(tm_min) << 16) + (static_cast<int64_t>(tm_sec) << 8);
        }

        // Cached epoch that is used to speedup comparators.
        int64_t _value;
    };

    static const Timestamp MIN_TIME(1, 1, 1900, 0, 0, 0);
    static const Timestamp MAX_TIME(1, 1, 2100, 0, 0, 0);

    bool operator==(const Timestamp t1, const Timestamp t2) { return t1._value == t2._value; }

    bool operator>(const Timestamp t1, const Timestamp t2) { return t1._value > t2._value; }

    bool operator<(const Timestamp t1, const Timestamp t2) { return t1._value < t2._value; }

    bool operator!=(const Timestamp t1, const Timestamp t2) { return t1._value != t2._value; }

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
} // namespace utils
