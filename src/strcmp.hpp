// SIMD implementations of == operator
// TODO: Implement > and < operator for std::string.
#pragma once
#include <cstring>
#include <immintrin.h>

namespace utils {
    namespace sse2 {
        // An SSE2 implementation of strncmp.
        bool strncmp(const char *first, const char *second, const size_t len) {
            size_t idx = 0;

            // Use SSE2 for the first len - (len%16) characters.
            constexpr size_t NBYTES = 16;
            const size_t res = len % NBYTES;
            const size_t N = len - res;
            if (N > 0) {
                for (; idx < N; idx += NBYTES) {
                    __m128i x = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + idx));
                    __m128i y =
                        _mm_loadu_si128(reinterpret_cast<const __m128i *>(second + idx));
                    __m128i results = _mm_cmpeq_epi8(x, y);
                    const int mask = _mm_movemask_epi8(results);
                    if (mask) { return false; }
                }
            }

            // Fallback to memcmp for the rest.
            return memcmp(first + idx, second + idx, res) == 0;
        }

    } // namespace sse2

#ifdef USE_AVX2
    namespace avx2 {
        // An AVX2 implementation of strncmp.
        bool strncmp(const char *first, const char *second, const size_t len) {
            size_t idx = 0;

            // Use AVX2 for the first len - (len %32) characters.
            constexpr size_t NBYTES = 32;
            const size_t res = len % NBYTES;
            const size_t N = len - res;
            if (N > 0) {
                for (; idx < N; idx += NBYTES) {
                    __m256i x =
                        _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + idx));
                    __m256i y =
                        _mm256_loadu_si256(reinterpret_cast<const __m256i *>(second + idx));
                    __m256i results = _mm256_cmpeq_epi8(x, y);
                    const int mask = _mm256_movemask_epi8(results);
                    if (mask) { return false; }
                }
            }

            // Fallback to strncmp_sse2 for the rest.
            return sse2::strncmp(first + idx, second + idx, res);
        }
    } // namespace avx2
#endif

} // namespace utils
