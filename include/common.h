#ifndef __COMMON_H__
#define __COMMON_H__

#include <inttypes.h>

static inline int32_t minimum(int32_t a, int32_t b);
static inline int32_t minimum(int32_t a, int32_t b)
{
    return a < b ? a : b;
}

static inline int32_t maximum(int32_t a, int32_t b);
static inline int32_t maximum(int32_t a, int32_t b)
{
    return a > b ? a : b;
}

static inline int32_t clamp(int32_t value, int32_t min, int32_t max);
static inline int32_t clamp(int32_t value, int32_t min, int32_t max)
{
    return maximum(minimum((value), (max)), (min));
}

#endif