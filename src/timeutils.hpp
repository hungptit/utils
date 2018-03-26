#pragma once

#include <ctime>
#include <string>

namespace utils {
    // A simple parser for timestamp.
    namespace timestamp {
        namespace {
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

            int parse_two_digits(const char *begin) {
                const char *ptr = begin;
                return ptr[0] * 10 + ptr[1] - SHIFT_2;
            }

            int parse_four_digits(const char *begin) {
                const char *ptr = begin;
                return ptr[0] * 1000 + ptr[1] * 100 + ptr[2] * 10 + ptr[3] - SHIFT_4;
            }
        }

        // Scribe log message has this format 02/04/2018 23:42:22.
        struct scribe {
            std::time_t operator()(const char *ptr) {
                std::time_t t;

                return t;
            }
            struct tm tm;
        };

        struct rabbitmq {};
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
