#ifndef __OSC_H__
#define __OSC_H__

#include <pico/stdio.h>
#include "phasor.h"

typedef struct sin_osc
{
    phasor_t phasor;
} sine_osc_t;

int16_t sine_osc_get_sample(sine_osc_t * o, int32_t phase_mod_input);
void sine_osc_process(sine_osc_t * o);
void sine_osc_set_pitch(sine_osc_t * o, int8_t octave, int16_t semitone, int16_t fine_tune);

#endif