#pragma once
#include <immintrin.h>

// An AVX2 implementation of memchr
const char *memchr_avx2(const char *ptr, char c, const size_t len) {
    __m256i pattern = _mm256_set1_epi8(c);
    size_t idx = 0;
    constexpr size_t NBYTES = 32;
    for (; idx + NBYTES < len; idx += NBYTES) {
        __m256i data = _mm256_lddqu_si256(reinterpret_cast<const __m256i *>(ptr + idx));
        __m256i results = _mm256_cmpeq_epi8(data, pattern);
        int mask = _mm256_movemask_epi8(results);
        if (mask) return ptr + idx + __builtin_ffs(mask) - 1;
    }

    // Check the last (len % 32) last characters
    for (; idx < len; ++idx) {
        if (*(ptr + idx) == c) return (ptr + idx);
    }

	// Return NULL if we could not find the given character.
    return NULL;
}
