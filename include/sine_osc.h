#ifndef __OSC_H__
#define __OSC_H__

#include <pico/stdio.h>
#include "phasor.h"
#include "signal.h"
#include "trig_gate.h"

typedef struct sine_osc_params
{
    int8_t octave_offset;
    int8_t transpose;
    int8_t fine_offset;
    int8_t mod_amount;
    audio_input_t * a_in;
} sine_osc_params_t;

typedef struct sin_osc
{
    phasor_t phasor;
    trig_t trig;
    sine_osc_params_t * params;
    audio_output_t a_out;
} sine_osc_t;

void sine_osc_params_set_octave_offset(sine_osc_params_t * p, int8_t octave_offset);
void sine_osc_params_set_transpose(sine_osc_params_t * p, int8_t transpose);
void sine_osc_params_set_fine_offset(sine_osc_params_t * p, int8_t fine_offset);
void sine_osc_params_set_mod_amount(sine_osc_params_t * p, int8_t mod_amount);
void sine_osc_params_attach_audio_input(sine_osc_params_t * o, audio_output_t * src);
void sine_osc_params_attach(sine_osc_t * o, sine_osc_params_t * p);

void sine_osc_process(sine_osc_t * o);
void sine_osc_process_params(sine_osc_t * o, int8_t midi_note);
void sine_osc_set_pitch(sine_osc_t * o, int8_t octave, int16_t semitone, int16_t fine_tune);
audio_output_t * sine_osc_get_output(sine_osc_t * o);
void sine_osc_attach_gate(sine_osc_t * o, gate_t * gate);
#endif