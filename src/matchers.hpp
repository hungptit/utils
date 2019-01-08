#pragma once

#include "string.hpp"
#include <cstring>
#include <string>

#ifdef USE_AVX2
#include "matchers_avx2.hpp"
#endif

#include "matchers_sse2.hpp"

namespace utils {
    // String matchers we should make our mathers as fast as we can.
    class AllPatterns {
      public:
        bool operator()(const char *, const char *) { return true; }
    };

#ifdef USE_AVX2
    using ExactMatcher = utils::avx2::ExactMatcher;
    using ExactMatcherInv = utils::avx2::ExactMatcherInv;
#else
    using ExactMatcher = utils::sse2::ExactMatcher;
    using ExactMatcherInv = utils::sse2::ExactMatcherInv;
#endif

} // namespace utils
