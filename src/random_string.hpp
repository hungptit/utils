#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <random>
#include <string>

namespace utils {
    /**
     * A simple implementation for a random string generator.
     */
    struct RandomStringGenerator {
      public:
        explicit RandomStringGenerator();
        explicit RandomStringGenerator(std::uint32_t seed);
        std::string operator()(const size_t len);

      private:
        std::mt19937 rgn;
        static constexpr int N = 64;
        static constexpr std::array<char, N> valid_characters = {
            {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
             'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F',
             'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
             'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '_', '.'}};
    };

} // namespace utils
