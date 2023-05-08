#ifndef __OSC_H__
#define __OSC_H__

#include <pico/stdio.h>
#include "phasor.h"
#include "a_io.h"
#include "trig_gate.h"

typedef struct sine_osc_params
{
    int8_t octave_offset;
    int8_t transpose;
    int8_t fine_offset;
    int8_t mod_amount;
    signal_src_t src;
} sine_osc_params_t;

typedef struct sin_osc
{
    phasor_t phasor;
    trig_t trig;
    sine_osc_params_t * params;
} sine_osc_t;

void sine_osc_params_set_octave_offset(sine_osc_params_t * p, int8_t m_val);
void sine_osc_params_set_transpose(sine_osc_params_t * p, int8_t m_val);
void sine_osc_params_set_fine_offset(sine_osc_params_t * p, int8_t m_val);
void sine_osc_params_set_mod_amount(sine_osc_params_t * p, int8_t m_val);
void sine_osc_params_attach(sine_osc_t * o, sine_osc_params_t * p);

void sine_osc_process(sine_osc_t * o, uint8_t voice, uint8_t op);
void sine_osc_process_params(sine_osc_t * o, int8_t midi_note);
void sine_osc_set_pitch(sine_osc_t * o, int8_t octave, int16_t semitone, int16_t fine_tune);
void sine_osc_reset_phase(sine_osc_t * o);
#endif