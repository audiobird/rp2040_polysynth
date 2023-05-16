#ifndef __OPERATOR_H__
#define __OPERATOR_H__

#include <pico/stdio.h>
#include "vca.h"
#include "sine_osc.h"
#include "adsr.h"
#include "a_io.h"
#include "trig_gate.h"

typedef struct operator_params
{
    adsr_params_t amp_adsr_params;
    adsr_params_t pitch_asr_params;
    vca_params_t vca_params;
    sine_osc_params_t sine_osc_params;
    signal_src_t src;
} operator_params_t;

typedef struct operator
{
    operator_params_t * params;
    adsr_t amp_adsr;
    adsr_t pitch_asr;
    vca_t vca;
    sine_osc_t sine_osc;
} operator_t;

void operator_params_attach(uint8_t voice, uint8_t op, operator_params_t * params);
void operator_process_params(uint8_t voice, int8_t midi_note);
void operator_process_audio(uint8_t voice);
void operator_init(operator_t * o, operator_params_t * params);
void operator_start(operator_t * o);

#endif