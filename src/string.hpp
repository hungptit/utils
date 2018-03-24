// This file has Wojciech Mula's sse2 and avx2 implementations of strstr
// functions. See this page https://github.com/WojciechMula/sse4-strstr for more
// information.
#pragma once
#include <cassert>
#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <immintrin.h>
#include <string>

#define FORCE_INLINE inline __attribute__((always_inline))
#define MAYBE_UNUSED inline __attribute__((unused))

namespace {

    MAYBE_UNUSED
    bool always_true(const char *, const char *) { return true; }

    MAYBE_UNUSED
    bool memcmp1(const char *a, const char *b) { return a[0] == b[0]; }

    MAYBE_UNUSED
    bool memcmp2(const char *a, const char *b) {
        const uint16_t A = *reinterpret_cast<const uint16_t *>(a);
        const uint16_t B = *reinterpret_cast<const uint16_t *>(b);
        return A == B;
    }

    MAYBE_UNUSED
    bool memcmp3(const char *a, const char *b) {

#ifdef USE_SIMPLE_MEMCMP
        return memcmp2(a, b) && memcmp1(a + 2, b + 2);
#else
        const uint32_t A = *reinterpret_cast<const uint32_t *>(a);
        const uint32_t B = *reinterpret_cast<const uint32_t *>(b);
        return (A & 0x00ffffff) == (B & 0x00ffffff);
#endif
    }

    MAYBE_UNUSED
    bool memcmp4(const char *a, const char *b) {

        const uint32_t A = *reinterpret_cast<const uint32_t *>(a);
        const uint32_t B = *reinterpret_cast<const uint32_t *>(b);
        return A == B;
    }

    MAYBE_UNUSED
    bool memcmp5(const char *a, const char *b) {

#ifdef USE_SIMPLE_MEMCMP
        return memcmp4(a, b) && memcmp1(a + 4, b + 4);
#else
        const uint64_t A = *reinterpret_cast<const uint64_t *>(a);
        const uint64_t B = *reinterpret_cast<const uint64_t *>(b);
        return ((A ^ B) & 0x000000fffffffffflu) == 0;
#endif
    }

    MAYBE_UNUSED
    bool memcmp6(const char *a, const char *b) {

#ifdef USE_SIMPLE_MEMCMP
        return memcmp4(a, b) && memcmp2(a + 4, b + 4);
#else
        const uint64_t A = *reinterpret_cast<const uint64_t *>(a);
        const uint64_t B = *reinterpret_cast<const uint64_t *>(b);
        return ((A ^ B) & 0x0000fffffffffffflu) == 0;
#endif
    }

    MAYBE_UNUSED
    bool memcmp7(const char *a, const char *b) {

#ifdef USE_SIMPLE_MEMCMP
        return memcmp4(a, b) && memcmp3(a + 4, b + 4);
#else
        const uint64_t A = *reinterpret_cast<const uint64_t *>(a);
        const uint64_t B = *reinterpret_cast<const uint64_t *>(b);
        return ((A ^ B) & 0x00fffffffffffffflu) == 0;
#endif
    }

    MAYBE_UNUSED
    bool memcmp8(const char *a, const char *b) {

        const uint64_t A = *reinterpret_cast<const uint64_t *>(a);
        const uint64_t B = *reinterpret_cast<const uint64_t *>(b);
        return A == B;
    }

    MAYBE_UNUSED
    bool memcmp9(const char *a, const char *b) {

        const uint64_t A = *reinterpret_cast<const uint64_t *>(a);
        const uint64_t B = *reinterpret_cast<const uint64_t *>(b);
        return (A == B) & (a[8] == b[8]);
    }

    MAYBE_UNUSED
    bool memcmp10(const char *a, const char *b) {

        const uint64_t Aq = *reinterpret_cast<const uint64_t *>(a);
        const uint64_t Bq = *reinterpret_cast<const uint64_t *>(b);
        const uint16_t Aw = *reinterpret_cast<const uint16_t *>(a + 8);
        const uint16_t Bw = *reinterpret_cast<const uint16_t *>(b + 8);
        return (Aq == Bq) & (Aw == Bw);
    }

    MAYBE_UNUSED
    bool memcmp11(const char *a, const char *b) {

#ifdef USE_SIMPLE_MEMCMP
        return memcmp8(a, b) && memcmp3(a + 8, b + 8);
#else
        const uint64_t Aq = *reinterpret_cast<const uint64_t *>(a);
        const uint64_t Bq = *reinterpret_cast<const uint64_t *>(b);
        const uint32_t Ad = *reinterpret_cast<const uint32_t *>(a + 8);
        const uint32_t Bd = *reinterpret_cast<const uint32_t *>(b + 8);
        return (Aq == Bq) & ((Ad & 0x00ffffff) == (Bd & 0x00ffffff));
#endif
    }

    MAYBE_UNUSED
    bool memcmp12(const char *a, const char *b) {

        const uint64_t Aq = *reinterpret_cast<const uint64_t *>(a);
        const uint64_t Bq = *reinterpret_cast<const uint64_t *>(b);
        const uint32_t Ad = *reinterpret_cast<const uint32_t *>(a + 8);
        const uint32_t Bd = *reinterpret_cast<const uint32_t *>(b + 8);
        return (Aq == Bq) & (Ad == Bd);
    }
} // namespace

namespace utils {
    namespace bits {
        template <typename T> T clear_leftmost_set(const T value) {
            assert(value != 0);
            return value & (value - 1);
        }

        template <typename T> unsigned get_first_bit_set(const T value) {
            assert(value != 0);
            return __builtin_ctz(value);
        }

        template <> unsigned get_first_bit_set<uint64_t>(const uint64_t value) {
            assert(value != 0);
            return __builtin_ctzl(value);
        }
    } // namespace bits

    namespace sse2 {
        size_t sse2_strstr_anysize(const char *s, size_t n, const char *needle, size_t k) {

            assert(k > 0);
            assert(n > 0);

            const __m128i first = _mm_set1_epi8(needle[0]);
            const __m128i last = _mm_set1_epi8(needle[k - 1]);

            for (size_t i = 0; i < n; i += 16) {

                const __m128i block_first =
                    _mm_loadu_si128(reinterpret_cast<const __m128i *>(s + i));
                const __m128i block_last =
                    _mm_loadu_si128(reinterpret_cast<const __m128i *>(s + i + k - 1));

                const __m128i eq_first = _mm_cmpeq_epi8(first, block_first);
                const __m128i eq_last = _mm_cmpeq_epi8(last, block_last);

                uint16_t mask = _mm_movemask_epi8(_mm_and_si128(eq_first, eq_last));

                while (mask != 0) {

                    const auto bitpos = bits::get_first_bit_set(mask);

                    if (memcmp(s + i + bitpos + 1, needle + 1, k - 2) == 0) {
                        return i + bitpos;
                    }

                    mask = bits::clear_leftmost_set(mask);
                }
            }

            return std::string::npos;
        }

        // ------------------------------------------------------------------------

        template <size_t k, typename MEMCMP>
        size_t sse2_strstr_memcmp(const char *s, size_t n, const char *needle,
                                  MEMCMP memcmp_fun) {

            assert(k > 0);
            assert(n > 0);

            const __m128i first = _mm_set1_epi8(needle[0]);
            const __m128i last = _mm_set1_epi8(needle[k - 1]);

            for (size_t i = 0; i < n; i += 16) {

                const __m128i block_first =
                    _mm_loadu_si128(reinterpret_cast<const __m128i *>(s + i));
                const __m128i block_last =
                    _mm_loadu_si128(reinterpret_cast<const __m128i *>(s + i + k - 1));

                const __m128i eq_first = _mm_cmpeq_epi8(first, block_first);
                const __m128i eq_last = _mm_cmpeq_epi8(last, block_last);

                uint32_t mask = _mm_movemask_epi8(_mm_and_si128(eq_first, eq_last));

                while (mask != 0) {

                    const auto bitpos = bits::get_first_bit_set(mask);

                    if (memcmp_fun(s + i + bitpos + 1, needle + 1)) { return i + bitpos; }

                    mask = bits::clear_leftmost_set(mask);
                }
            }

            return std::string::npos;
        }

        // ------------------------------------------------------------------------

        size_t sse2_strstr_v2(const char *s, size_t n, const char *needle, size_t k) {

            size_t result = std::string::npos;

            if (n < k) { return result; }

            switch (k) {
            case 0: return 0;

            case 1: {
                const char *res = reinterpret_cast<const char *>(strchr(s, needle[0]));

                return (res != nullptr) ? res - s : std::string::npos;
            }

            case 2: result = sse2_strstr_memcmp<2>(s, n, needle, always_true); break;

            case 3: result = sse2_strstr_memcmp<3>(s, n, needle, memcmp1); break;

            case 4: result = sse2_strstr_memcmp<4>(s, n, needle, memcmp2); break;

            case 5: result = sse2_strstr_memcmp<5>(s, n, needle, memcmp4); break;

            case 6: result = sse2_strstr_memcmp<6>(s, n, needle, memcmp4); break;

            case 7: result = sse2_strstr_memcmp<7>(s, n, needle, memcmp5); break;

            case 8: result = sse2_strstr_memcmp<8>(s, n, needle, memcmp6); break;

            case 9: result = sse2_strstr_memcmp<9>(s, n, needle, memcmp8); break;

            case 10: result = sse2_strstr_memcmp<10>(s, n, needle, memcmp8); break;

            case 11: result = sse2_strstr_memcmp<11>(s, n, needle, memcmp9); break;

            case 12: result = sse2_strstr_memcmp<12>(s, n, needle, memcmp10); break;

            default: result = sse2_strstr_anysize(s, n, needle, k); break;
            }

            if (result <= n - k) {
                return result;
            } else {
                return std::string::npos;
            }
        }

    } // namespace sse2
    namespace avx2 {
        // implements scheme described in
        // http://0x80.pl/articles/simd-friendly-karp-rabin.html

        size_t FORCE_INLINE avx2_strstr_anysize(const char *s, size_t n, const char *needle,
                                                size_t k) {

            assert(k > 0);
            assert(n > 0);

            const __m256i first = _mm256_set1_epi8(needle[0]);
            const __m256i last = _mm256_set1_epi8(needle[k - 1]);

            for (size_t i = 0; i < n; i += 32) {

                const __m256i block_first =
                    _mm256_loadu_si256(reinterpret_cast<const __m256i *>(s + i));
                const __m256i block_last =
                    _mm256_loadu_si256(reinterpret_cast<const __m256i *>(s + i + k - 1));

                const __m256i eq_first = _mm256_cmpeq_epi8(first, block_first);
                const __m256i eq_last = _mm256_cmpeq_epi8(last, block_last);

                uint32_t mask = _mm256_movemask_epi8(_mm256_and_si256(eq_first, eq_last));

                while (mask != 0) {

                    const auto bitpos = bits::get_first_bit_set(mask);

                    if (memcmp(s + i + bitpos + 1, needle + 1, k - 2) == 0) {
                        return i + bitpos;
                    }

                    mask = bits::clear_leftmost_set(mask);
                }
            }

            return std::string::npos;
        }

#include "avx2-strstr-v2-clang-specific.cpp"

        template <size_t K>
        size_t FORCE_INLINE avx2_strstr_eq(const char *s, size_t n, const char *needle) {

            static_assert(K > 0 && K < 16, "K must be in range [1..15]");
            assert(n > 0);

            __m256i broadcasted[K];
            for (unsigned i = 0; i < K; i++) { broadcasted[i] = _mm256_set1_epi8(needle[i]); }

            __m256i curr = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(s));

            for (size_t i = 0; i < n; i += 32) {

                const __m256i next =
                    _mm256_loadu_si256(reinterpret_cast<const __m256i *>(s + i + 32));

                __m256i eq = _mm256_cmpeq_epi8(curr, broadcasted[0]);

                // AVX2 palignr works on 128-bit lanes, thus some extra work is needed
                //
                // curr = [a, b] (2 x 128 bit)
                // next = [c, d]
                // substring = [palignr(b, a, i), palignr(c, b, i)]
                __m256i next1;
                next1 = _mm256_inserti128_si256(next1, _mm256_extracti128_si256(curr, 1),
                                                0); // b
                next1 = _mm256_inserti128_si256(next1, _mm256_extracti128_si256(next, 0),
                                                1); // c

#ifndef __clang__
                for (unsigned i = 1; i < K; i++) {
                    const __m256i substring = _mm256_alignr_epi8(next1, curr, i);
                    eq = _mm256_and_si256(eq, _mm256_cmpeq_epi8(substring, broadcasted[i]));
                }
#else
                inner_loop<K>()(eq, next1, curr, broadcasted);
#endif

                curr = next;

                const uint32_t mask = _mm256_movemask_epi8(eq);
                if (mask != 0) { return i + bits::get_first_bit_set(mask); }
            }

            return std::string::npos;
        }

        template <size_t k, typename MEMCMP>
        size_t FORCE_INLINE avx2_strstr_memcmp(const char *s, size_t n, const char *needle,
                                               MEMCMP memcmp_fun) {

            assert(k > 0);
            assert(n > 0);

            const __m256i first = _mm256_set1_epi8(needle[0]);
            const __m256i last = _mm256_set1_epi8(needle[k - 1]);

            for (size_t i = 0; i < n; i += 32) {

                const __m256i block_first =
                    _mm256_loadu_si256(reinterpret_cast<const __m256i *>(s + i));
                const __m256i block_last =
                    _mm256_loadu_si256(reinterpret_cast<const __m256i *>(s + i + k - 1));

                const __m256i eq_first = _mm256_cmpeq_epi8(first, block_first);
                const __m256i eq_last = _mm256_cmpeq_epi8(last, block_last);

                uint32_t mask = _mm256_movemask_epi8(_mm256_and_si256(eq_first, eq_last));

                while (mask != 0) {

                    const auto bitpos = bits::get_first_bit_set(mask);

                    if (memcmp_fun(s + i + bitpos + 1, needle + 1)) { return i + bitpos; }

                    mask = bits::clear_leftmost_set(mask);
                }
            }

            return std::string::npos;
        }

        size_t avx2_strstr_v2(const char *s, size_t n, const char *needle, size_t k) {

            size_t result = std::string::npos;

            if (n < k) { return result; }

            switch (k) {
            case 0: return 0;

            case 1: {
                const char *res = reinterpret_cast<const char *>(strchr(s, needle[0]));

                return (res != nullptr) ? res - s : std::string::npos;
            }

            case 2: result = avx2_strstr_eq<2>(s, n, needle); break;

            case 3: result = avx2_strstr_memcmp<3>(s, n, needle, memcmp1); break;

            case 4: result = avx2_strstr_memcmp<4>(s, n, needle, memcmp2); break;

            case 5:
                // Note: use memcmp4 rather memcmp3, as the last character
                //       of needle is already proven to be equal
                result = avx2_strstr_memcmp<5>(s, n, needle, memcmp4);
                break;

            case 6: result = avx2_strstr_memcmp<6>(s, n, needle, memcmp4); break;

            case 7: result = avx2_strstr_memcmp<7>(s, n, needle, memcmp5); break;

            case 8: result = avx2_strstr_memcmp<8>(s, n, needle, memcmp6); break;

            case 9:
                // Note: use memcmp8 rather memcmp7 for the same reason as above.
                result = avx2_strstr_memcmp<9>(s, n, needle, memcmp8);
                break;

            case 10: result = avx2_strstr_memcmp<10>(s, n, needle, memcmp8); break;

            case 11: result = avx2_strstr_memcmp<11>(s, n, needle, memcmp9); break;

            case 12: result = avx2_strstr_memcmp<12>(s, n, needle, memcmp10); break;

            default: result = avx2_strstr_anysize(s, n, needle, k); break;
            }

            if (result <= n - k) {
                return result;
            } else {
                return std::string::npos;
            }
        }

        size_t avx2_strstr_v2(const std::string &s, const std::string &needle) {
            return avx2_strstr_v2(s.data(), s.size(), needle.data(), needle.size());
        }
    } // namespace avx2
} // namespace utils
