#pragma once

#include <ctime>
#include <string>

namespace utils {
    // A simple parser for timestamp.
    namespace timestamp {
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

        // Scribe log message has this format "02/04/2018 23:42:22".
        // This parse doesn't validate seperator characters.
        struct scribe {
            std::time_t operator()(const char *p) {
                const char *ptr = p;

                tm.tm_mon = parse_digits<2>(ptr, 0) - SHIFT_MONTH;
                ptr += 3;

                tm.tm_mday = parse_digits<2>(ptr, 0);
                ptr += 3;

                tm.tm_year = parse_digits<4>(ptr, 0) - SHIFT_YEAR;

                ptr += 5;
                tm.tm_hour = parse_digits<2>(ptr, 0);

                ptr += 3;
                tm.tm_min = parse_digits<2>(ptr, 0);

                ptr += 3;
                tm.tm_sec = parse_digits<2>(ptr, 0);

                tm.tm_isdst = 0; // Do not care about day light saving.
                return mktime(&tm);
            }
            struct tm tm;
        };

        struct rabbitmq {};

        struct AllTimestamps {
            bool operator()(const std::time_t t) { return true; }
        };

        struct OlderThan {
            explicit OlderThan(const std::time_t t) : timestamp(t) {}
            bool operator()(const std::time_t t) { return t > timestamp; }
            const std::time_t timestamp;
        };

        struct NewerThan {
			explicit NewerThan(const std::time_t t) : timestamp(t){}
			bool operator()(const std::time_t t) {
				return t < timestamp;
			}
			const std::time_t timestamp;
		};

        struct Equals {
			explicit Equals(const std::time_t t) : timestamp(t){}
			bool operator()(const std::time_t t) {
				return t == timestamp;
			}
			const std::time_t timestamp;
		};

        struct Between {
			Between(const std::time_t b, const std::time_t e) : begin(b), end(e){}

			bool operator()(const std::time_t t) {
				return (t > begin) && (t < end);
			}
			
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
