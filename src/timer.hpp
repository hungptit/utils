#pragma once
#include <chrono>
#include <iostream>
#include <string>

namespace utils {
    enum DispOpt { MICROSECOND = 1000000, MILLISECOND = 1000, SECOND = 1 };
    template <DispOpt val> std::string getUnitString();

    template <> std::string getUnitString<MICROSECOND>() { return " microseconds"; }
    template <> std::string getUnitString<MILLISECOND>() { return " milliseconds"; }
    template <> std::string getUnitString<SECOND>() { return " seconds"; }

    class Timer {
      public:
        Timer() {
            tic();
            TicksPerSeconds = clock::duration::period::den / clock::duration::period::num;
        }

        Timer(Timer &) = delete;
        Timer(Timer &&) = delete;
        Timer &operator=(Timer &) = delete;

        void tic() { StartTime = clock::now(); }

        auto toc() const { return (clock::now() - StartTime).count(); }

        auto ticksPerSecond() const { return TicksPerSeconds; }

      private:
        using clock = std::chrono::high_resolution_clock;
        clock::time_point StartTime;
        double TicksPerSeconds;
    };

    template <DispOpt val = SECOND> class ElapsedTime {
      public:
        explicit ElapsedTime() : LocalTimer(), Message("Elapsed time: "), Verbose(true) {}
        explicit ElapsedTime(const std::string &msg)
            : LocalTimer(), Message(msg), Verbose(true) {}
        explicit ElapsedTime(const std::string &msg, bool verbose)
            : LocalTimer(), Message(msg), Verbose(verbose) {}

        ~ElapsedTime() {
            if (Verbose) {
                std::cout << Message << LocalTimer.toc() * val / LocalTimer.ticksPerSecond()
                          << getUnitString<val>() << std::endl;
            }
        }

      private:
        Timer LocalTimer;
        std::string Message;
        bool Verbose;
    };
} // namespace utils
