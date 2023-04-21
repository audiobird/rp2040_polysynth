#ifndef __OPERATOR_H__
#define __OPERATOR_H__

#include <pico/stdio.h>
#include "vca.h"
#include "sine_osc.h"
#include "adsr.h"
#include "signal.h"
#include "trig_gate.h"

typedef struct operator_params
{
    adsr_params_t amp_adsr_params;
    vca_params_t vca_params;
    sine_osc_params_t sine_osc_params;
} operator_params_t;

typedef struct operator
{
    operator_params_t * params;
    adsr_t amp_adsr;
    vca_t vca;
    sine_osc_t sine_osc;
    audio_output_t a_out;
} operator_t;

void operator_params_attach(operator_t * operator, operator_params_t * params);
void operator_process_params(operator_t * op, int8_t midi_note);
void operator_process_audio(operator_t * op);
void operator_params_attach_audio_input(operator_params_t * params, audio_output_t * src);
audio_output_t * operator_get_output(operator_t * op);
void operator_init(operator_t * operator, operator_params_t * params);

#endif