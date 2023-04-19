#ifndef __PHASOR_H__
#define __PHASOR_H__

#include <pico/stdio.h>

#define SYNTH_NUM_PHASORS (SYNTH_NUM_VOICES * SYNTH_OPERATORS_PER_VOICE)
#define M_NOTE_TO_PHASE_TABLE(a) (uint16_t)(a << (PHASE_STEP_TABLE_BIT_SIZE - 7))
#define PHASE_STEP_TABLE_SIZE (1u << PHASE_STEP_TABLE_BIT_SIZE)

typedef struct phasor
{
    uint32_t accumulator;
    uint32_t phase_step;
} phasor_t;

typedef int16_t phasor_tuning_word_t;

void phasor_advance_phase(phasor_t *);
uint16_t phasor_get_phase(phasor_t *);
void phasor_set_phase_step( phasor_t *, phasor_tuning_word_t);
void phasor_reset_phase(phasor_t *);
phasor_tuning_word_t phasor_create_tuning_word(int8_t, int16_t, int16_t);

#endif
