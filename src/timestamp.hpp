#pragma once

#include <ctime>

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

    namespace detail {
        struct _ts {
            _ts()
                : tm_isdst(0), tm_sec(0), tm_min(0), tm_hour(0), tm_mday(1), tm_mon(1),
                  tm_year(1900) {}

            _ts(const unsigned short year, const unsigned char month, const unsigned short day,
                const unsigned char hour, const unsigned char minute,
                const unsigned char second)
                : tm_isdst(0), tm_sec(second), tm_min(minute), tm_hour(hour), tm_mday(day),
                  tm_mon(month), tm_year(year) {}

            template <typename T>
            _ts(T &&t)
                : tm_isdst(t.tm_isdst), tm_sec(t.tm_sec), tm_min(t.tm_min), tm_hour(t.tm_hour),
                  tm_mday(t.tm_mday), tm_mon(t.tm_mon), tm_year(t.tm_year) {}

            unsigned char tm_isdst;
            unsigned char tm_sec;
            unsigned char tm_min;
            unsigned char tm_hour;
            unsigned char tm_mday;
            unsigned char tm_mon;
            unsigned short tm_year;
        };

        union _tsdata {
            _tsdata() : ts() {}
            explicit _tsdata(const unsigned short year, const unsigned char mon,
                             const unsigned char day, const unsigned char hour,
                             const unsigned char minute, const unsigned char second) noexcept
                : ts(year, mon, day, hour, minute, second) {}
            template <typename T> _tsdata(T &&t) : value(t.value) {}
            detail::_ts ts;
            int64_t value;
        };
    } // namespace detail

    // A simple data structure to hold time information. This data structure
    // must be similar to struct tm so that we can reuse our parsing code.
    struct Timestamp {
        explicit Timestamp() noexcept : data() {}

        explicit Timestamp(const unsigned short year, const unsigned char mon,
                           const unsigned char day, const unsigned char hour,
                           const unsigned char minute, const unsigned char second) noexcept
            : data(year, mon, day, hour, minute, second) {
        }

        // Copy and move constructor won't recompute _value.
        template <typename T> Timestamp(T &&t) noexcept : data(t.data) {}

        // Convert to the tm struct. This is very useful when we want to print out the
        // timestamp.
        struct tm to_tm() const {
            struct tm tm;
            tm.tm_sec = tm_sec();
            tm.tm_min = tm_min();
            tm.tm_hour = tm_hour();
            tm.tm_mday = tm_mday();
            tm.tm_mon = tm_mon() - SHIFT_MONTH;
            tm.tm_year = tm_year() - SHIFT_YEAR;
            tm.tm_isdst = data.ts.tm_isdst;
            return tm;
        }

        // Get methods
        int tm_year() const { return data.ts.tm_year; }
        int tm_mon() const { return data.ts.tm_mon; }
        int tm_mday() const { return data.ts.tm_mday; }
        int tm_hour() const { return data.ts.tm_hour; }
        int tm_min() const { return data.ts.tm_min; }
        int tm_sec() const { return data.ts.tm_sec; }
        int tm_isdst() const { return data.ts.tm_isdst; }

        detail::_tsdata data;
    };

    static const Timestamp MIN_TIME(1900, 1, 1, 0, 0, 0);
    static const Timestamp MAX_TIME(2100, 1, 1, 0, 0, 0);

    bool operator==(const Timestamp t1, const Timestamp t2) {
        return t1.data.value == t2.data.value;
    }

    bool operator>(const Timestamp t1, const Timestamp t2) {
        return t1.data.value > t2.data.value;
    }

    bool operator<(const Timestamp t1, const Timestamp t2) {
        return t1.data.value < t2.data.value;
    }

    bool operator!=(const Timestamp t1, const Timestamp t2) {
        return t1.data.value != t2.data.value;
    }

    // A timestamp string will be in this format "MM/DD/YYYY hh::mm::ss"
    template <typename T> T parse_scribe_timestamp(const char *ptr) {
        T tm;
        tm.data.ts.tm_mon = parse_digits<2>(ptr, 0);
        tm.data.ts.tm_mday = parse_digits<2>(ptr + 3, 0);
        tm.data.ts.tm_year = parse_digits<4>(ptr + 6, 0);
        tm.data.ts.tm_hour = parse_digits<2>(ptr + 11, 0);
        tm.data.ts.tm_min = parse_digits<2>(ptr + 14, 0);
        tm.data.ts.tm_sec = parse_digits<2>(ptr + 17, 0);
        tm.data.ts.tm_isdst = 0; // We do not care about day light saving when parsing log data.
        return tm;
    }

    // Time constraints
    template <typename T> struct All {
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
