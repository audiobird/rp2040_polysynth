#include "phasor.h"
#include "tables/phase_step_table.h"
#include "common.h"

const uint8_t shift_val = 32 - WAVE_TABLE_BIT_WIDTH;

void phasor_advance_phase(phasor_t * p)
{
    p->accumulator += p->phase_step;
}

uint16_t phasor_get_phase(phasor_t * p)
{
    return p->accumulator >> shift_val;
}

void phasor_reset_phase(phasor_t * p)
{
    p->accumulator = 0;
}

void phasor_set_phase_step( phasor_t * p, phasor_tuning_word_t t)
{
    p->phase_step = phase_step_table[t];
}

phasor_tuning_word_t phasor_create_tuning_word(int8_t octave, int16_t semitone, int16_t fine_tune)
{
    int out = octave * 12;
    out += semitone;
    out <<= (PHASE_STEP_TABLE_BIT_SIZE - 7);
    out += fine_tune;
    return clamp(out, 0, PHASE_STEP_TABLE_SIZE - 1);
}