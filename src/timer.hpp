#pragma once
#include <chrono>
#include <iostream>
#include <string>

namespace utils {
    enum DispOpt { MICROSECOND = 1000000, MILLISECOND = 1000, SECOND = 1 };
    template <DispOpt val> std::string get_unit_string();
    template <> std::string get_unit_string<MICROSECOND>() { return " microseconds"; }
    template <> std::string get_unit_string<MILLISECOND>() { return " milliseconds"; }
    template <> std::string get_unit_string<SECOND>() { return " seconds"; }

    class Timer {
      public:
        Timer() {
            tic();
            ticks_per_seconds = clock::duration::period::den / clock::duration::period::num;
        }

        Timer(Timer &) = delete;
        Timer(Timer &&) = delete;
        Timer &operator=(Timer &) = delete;

        void tic() { start_time = clock::now(); }

        auto toc() const { return (clock::now() - start_time).count(); }

        auto ticksPerSecond() const { return ticks_per_seconds; }

      private:
        using clock = std::chrono::high_resolution_clock;
        clock::time_point start_time;
        double ticks_per_seconds;
    };

    template <DispOpt val = SECOND> class ElapsedTime {
      public:
        explicit ElapsedTime() : local_timer(), message("Elapsed time: "), verbose(true) {}
        explicit ElapsedTime(const std::string &msg)
            : local_timer(), message(msg), verbose(true) {}
        explicit ElapsedTime(const std::string &msg, bool verbose)
            : local_timer(), message(msg), verbose(verbose) {}

        ~ElapsedTime() {
            if (verbose) {
                std::cout << message << local_timer.toc() * val / local_timer.ticksPerSecond()
                          << get_unit_string<val>() << std::endl;
            }
        }

      private:
        Timer local_timer;
        std::string message;
        bool verbose;
    };
} // namespace utils
