#pragma once

#include "fmt/format.h"

namespace utils {
    template <typename OArchive, typename T>
    void data_dumper(T && data, const std::string &msg) {
        std::stringstream ss;
        {
            OArchive oarchive(ss);
            oarchive(cereal::make_nvp(msg, data));
        }
        fmt::print("{0}\n", ss.str());
    }
}
