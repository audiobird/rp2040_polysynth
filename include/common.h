#ifndef __COMMON_H__
#define __COMMON_H__

#include <inttypes.h>

static inline int32_t minimum(const int32_t a, const int32_t b);
static inline int32_t minimum(const int32_t a, const int32_t b)
{
    return a < b ? a : b;
}

static inline int32_t maximum(const int32_t a, const int32_t b);
static inline int32_t maximum(const int32_t a, const int32_t b)
{
    return a > b ? a : b;
}

static inline int32_t clamp(const int32_t value, const int32_t min, const int32_t max);
static inline int32_t clamp(const int32_t value, const int32_t min, const int32_t max)
{
    return maximum(minimum((value), (max)), (min));
}

static inline int32_t divide_signed_by_bits(const int32_t numerator, const uint8_t bits);
static inline int32_t divide_signed_by_bits(const int32_t numerator, const uint8_t bits)
{
    #ifdef DIVIDE_FAST_SHIFT
    return numerator >> bits;
    #elif DIVIDE_FAST_ADJUSTED
    return numerator / (1l << bits);
    #elif DIVIDE_ACCURATE
    return numerator / ((1l << bits) - 1);
    #endif
}

static inline int32_t multiply_and_scale(const int32_t x, const int32_t y, const uint8_t scale_bits);
static inline int32_t multiply_and_scale(const int32_t x, const int32_t y, const uint8_t scale_bits)
{
    int32_t out = x * y;
    return divide_signed_by_bits(out, scale_bits);
}

static inline int32_t audio_sum_and_clamp(const int32_t x, const int32_t y);
static inline int32_t audio_sum_and_clamp(const int32_t x, const int32_t y)
{
    return clamp(x + y, INT32_MIN, INT32_MAX); 
}

#endif