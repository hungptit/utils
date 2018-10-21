#pragma once

#include <cinttypes>
#include <cassert>

#define FORCE_INLINE inline __attribute__((always_inline))
#define MAYBE_UNUSED inline __attribute__((unused))

namespace utils {
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
}
