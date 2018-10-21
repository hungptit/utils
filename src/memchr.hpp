#pragma once
#include <cstring>
#include <immintrin.h>

#ifdef USE_AVX2
namespace utils {
    namespace avx2 {
        // An AVX2 implementation of memchr
        const char *memchr(const char *ptr, const char c, const size_t len) {
            size_t idx = 0;

            // Use AVX2 for the first len - (len %32) characters.
            constexpr size_t NBYTES = 32;
            const size_t res = len % 32;
            const size_t N = len - res;
            if (N > 0) {
                __m256i pattern = _mm256_set1_epi8(c);
                for (; idx < N; idx += NBYTES) {
                    __m256i data =
                        _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ptr + idx));
                    __m256i results = _mm256_cmpeq_epi8(data, pattern);
                    int mask = _mm256_movemask_epi8(results);
                    if (mask) { return ptr + idx + __builtin_ffs(mask) - 1; }
                }
            }

            // Fallback to standard memchr for the rest.
            return static_cast<const char *>(::memchr(ptr + idx, c, res));
        }
    } // namespace avx2
} // namespace utils
#endif
