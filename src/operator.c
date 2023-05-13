#include "operator.h"
#include "common.h"

const signal_dst_t op_dst[SYNTH_OPERATORS_PER_VOICE] = {SRC_OP0, SRC_OP1};

void operator_params_attach(operator_t * o, operator_params_t * params)
{
    o->params = params;
}

void operator_process_params(operator_t * o, int8_t midi_note)
{
    adsr_process(&o->amp_adsr);
    sine_osc_process_params(&o->sine_osc, midi_note);
}

void operator_process_audio(operator_t * o, uint8_t voice, uint8_t op)
{
    sine_osc_process(&o->sine_osc, voice, op);
    vca_process(&o->vca, voice, op);
    audio_output_t temp = audio_get_src_phase(voice, o->params->src);
    temp = multiply_and_scale(temp, adsr_get_output(&o->amp_adsr), 16);
    audio_set_dst_phase(voice, op_dst[op], temp);
}

void operator_init(operator_t * o, operator_params_t * params)
{
    operator_params_attach(o, params);
    sine_osc_params_attach(&o->sine_osc, &params->sine_osc_params);
    vca_params_attach(&o->vca, &params->vca_params);
    adsr_params_attach(&o->amp_adsr, &params->amp_adsr_params);
}

void operator_start(operator_t * o)
{
    adsr_start(&o->amp_adsr);   
}