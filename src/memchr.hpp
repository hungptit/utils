#pragma once
#include <cstring>
#include <immintrin.h>

// An AVX2 implementation of memchr
const char *memchr_avx2(const char *ptr, const char c, const size_t len) {
    size_t idx = 0;

    // Use AVX2 for the first len - (len %32) characters.
    constexpr size_t NBYTES = 32;
    const size_t res = len % 32;
    const size_t N = len - res;
    if (N > 0) {
        __m256i pattern = _mm256_set1_epi8(c);
        for (; idx < N; idx += NBYTES) {
            __m256i data = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(ptr + idx));
            __m256i results = _mm256_cmpeq_epi8(data, pattern);
            int mask = _mm256_movemask_epi8(results);
            if (mask) { return ptr + idx + __builtin_ffs(mask) - 1; }
        }
    }

    // Fallback to memchr for the rest.
    return static_cast<const char *>(memchr(ptr + idx, c, res));
}

const char *memchr_avx2_v2(const char *ptr, const char c, const size_t len) {

    if (len == 0) return nullptr;

    char *idx = const_cast<char *>(ptr);
    const char *last_pos = const_cast<char *>(ptr) + len;

    // Read bytes until memory is 16-byte aligned (128-bit)
    for (; (((size_t)idx & 15u) != 0) && (idx < last_pos); ++idx) {
        if (c == *(idx)) return idx;
    }

    const char *last_pos_m16 = last_pos - 16;
    __m256i pattern;

    // if memory location is not 32-byte aligned, read and check 1 128-bit register.
    if ((((size_t)idx & 31u) != 0) && (idx < last_pos_m16)) {
        __m128i pattern16 = _mm_set1_epi8(c);

        __m128i data_128 = _mm_load_si128(reinterpret_cast<const __m128i *>(idx));

        int mask = _mm_movemask_epi8(_mm_cmpeq_epi8(pattern16, data_128));

        if (mask) return idx + __builtin_ffs(mask) - 1;

        idx += 16;
        pattern = _mm256_broadcastsi128_si256(pattern16);
    } else {
        pattern = _mm256_set1_epi8(c);
    }

    const char *last_pos_m32 = last_pos - 32;
    __m256i data1;

    // if memory location is not 64-byte aligned, read and check 1 256-bit register.
    if ((((size_t)idx & 63u) != 0) && (idx < last_pos_m32)) {
        data1 = _mm256_load_si256(reinterpret_cast<const __m256i *>(idx));

        int mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(pattern, data1));

        if (mask) return idx + __builtin_ffs(mask) - 1;

        idx += 32;
    }

    __m256i data2;

    // if memory location is not 128-byte aligned, read and check 2 256-bit register.
    if ((((size_t)idx & 127u) != 0) && (idx < last_pos - 64)) {
        data1 = _mm256_load_si256(reinterpret_cast<const __m256i *>(idx));
        data2 = _mm256_load_si256(reinterpret_cast<const __m256i *>(idx + 32));

        int mask1 = _mm256_movemask_epi8(_mm256_cmpeq_epi8(pattern, data1));
        int mask2 = _mm256_movemask_epi8(_mm256_cmpeq_epi8(pattern, data2));

        if (mask1)
            return idx + __builtin_ffs(mask1) - 1;
        else if (mask2)
            return idx + __builtin_ffs(mask2) + 31;

        idx += 64;
    }

    const char *last_pos_m128 = last_pos - 128;

    __m256i results1, results2, results3, results4;

    // test 128 bytes at once
    for (; idx < last_pos_m128; idx += 128) {
        __m256i data1 = _mm256_load_si256(reinterpret_cast<const __m256i *>(idx));
        __m256i data2 = _mm256_load_si256(reinterpret_cast<const __m256i *>(idx + 32));
        __m256i data3 = _mm256_load_si256(reinterpret_cast<const __m256i *>(idx + 64));
        __m256i data4 = _mm256_load_si256(reinterpret_cast<const __m256i *>(idx + 96));

        results1 = _mm256_cmpeq_epi8(pattern, data1);
        results2 = _mm256_cmpeq_epi8(pattern, data2);
        results3 = _mm256_cmpeq_epi8(pattern, data3);
        results4 = _mm256_cmpeq_epi8(pattern, data4);

        __m256i tmp1 = _mm256_or_si256(results1, results2);
        __m256i tmp2 = _mm256_or_si256(results3, results4);
        __m256i tmp3 = _mm256_or_si256(tmp1, tmp2);

        if (_mm256_movemask_epi8(tmp3)) break;
    }

    if (idx < last_pos_m128) {
        // found something
        int mask1 = _mm256_movemask_epi8(results1);
        int mask2 = _mm256_movemask_epi8(results2);
        int mask3 = _mm256_movemask_epi8(results3);
        int mask4 = _mm256_movemask_epi8(results4);

        if (mask1)
            return idx + __builtin_ffs(mask1) - 1;
        else if (mask2)
            return idx + __builtin_ffs(mask2) + 31;
        else if (mask3)
            return idx + __builtin_ffs(mask3) + 63;
        else
            return idx + __builtin_ffs(mask4) + 95; // mask4
    }

    // Not find anything

    // Run 32-bytes until cannot
    for (; idx < last_pos_m32; idx += 32) {
        data1 = _mm256_load_si256(reinterpret_cast<const __m256i *>(idx));
        results1 = _mm256_cmpeq_epi8(data1, pattern);

        int mask = _mm256_movemask_epi8(results1);

        if (mask) return idx + __builtin_ffs(mask) - 1;
    }

    // Run 16-bytes if possible
    if (idx < last_pos_m16) {
        __m128i pattern1 = _mm256_castsi256_si128(pattern);
        __m128i data = _mm_load_si128(reinterpret_cast<const __m128i *>(idx));
        __m128i results = _mm_cmpeq_epi8(data, pattern1);
        int mask = _mm_movemask_epi8(results);
        if (mask) return idx + __builtin_ffs(mask) - 1;
        idx += 16;
    }

    // Last couple of bytes
    for (; idx < last_pos; ++idx) {
        if (*(idx) == c) return (idx);
    }

    return nullptr; // Return NULL if we can not find any match.
}
