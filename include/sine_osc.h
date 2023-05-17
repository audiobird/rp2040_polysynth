#ifndef __OSC_H__
#define __OSC_H__

#include <pico/stdio.h>
#include "a_io.h"

typedef struct sine_osc_params
{
    int16_t octave_offset;
    int16_t transpose;
    int8_t fine_offset;
    uint16_t mod_amount;
    bool track_pitch;
    signal_src_t src;
} sine_osc_params_t;

void sine_osc_process(uint8_t voice, uint8_t op);
void sine_osc_process_params(uint8_t voice, uint8_t op);

void sine_osc_params_set_octave_offset(sine_osc_params_t * p, int8_t m_val);
void sine_osc_params_set_transpose(sine_osc_params_t * p, int8_t m_val);
void sine_osc_params_set_fine_offset(sine_osc_params_t * p, int8_t m_val);
void sine_osc_params_set_mod_amount(sine_osc_params_t * p, int8_t m_val);
void sine_osc_params_set_pitch_tracking(sine_osc_params_t * p, int8_t m_val);
void sine_osc_trig_voice(uint8_t voice, int8_t mnote);

void sine_osc_params_attach(uint8_t voice, uint8_t op, sine_osc_params_t * params);
#endif