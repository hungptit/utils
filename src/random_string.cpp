#include <array>
#include <chrono>
#include <cstdint>
#include <random>
#include <string>

#include "random_string.hpp"

namespace utils {

    // Use the current time as a seed.
    RandomStringGenerator::RandomStringGenerator()
        : rgn(std::chrono::system_clock::now().time_since_epoch().count()) {}

    RandomStringGenerator::RandomStringGenerator(std::uint32_t seed) : rgn(seed) {}

    std::string RandomStringGenerator::operator()(const size_t len) {
        std::string str(len, 0);
        for (size_t idx = 0; idx < len; ++idx) { str[idx] = valid_characters[rgn() % N]; }
        return str;
    }
} // namespace utils
