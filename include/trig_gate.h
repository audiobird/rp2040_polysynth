#ifndef __TRIG_GATE_H__
#define __TRIG_GATE_H__

#include "pico/stdio.h"

typedef int32_t gate_t;

typedef struct trig
{
    gate_t * current_gate;
    gate_t prev_gate;
} trig_t;

static inline void gate_attach_src(gate_t * dst, gate_t * src);
static inline void gate_attach_src(gate_t * dst, gate_t * src)
{
    dst = src;
}

static inline void trig_attach_gate(trig_t * dst, gate_t * src);
static inline void trig_attach_gate(trig_t * dst, gate_t * src)
{
    gate_attach_src(dst->current_gate, src);
}

static inline bool is_trig(trig_t * t);
static inline bool is_trig(trig_t * t)
{
    if (!*t->current_gate)
    return t->prev_gate = 0;

    if (*t->current_gate && t->prev_gate)
    return 0;

    return t->prev_gate = 1;
}

#endif 