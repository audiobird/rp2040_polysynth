#include "operator.h"
#include "common.h"

const signal_dst_t op_dst[SYNTH_OPERATORS_PER_VOICE] = {SRC_OP0, SRC_OP1};

static operator_t o[SYNTH_NUM_VOICES][SYNTH_OPERATORS_PER_VOICE];

void operator_params_attach(uint8_t voice, uint8_t op, operator_params_t * params)
{
    o[voice][op].params = params;
    sine_osc_params_attach(voice, op, &params->sine_osc_params)
}

void operator_process_params(uint8_t voice, uint8_t op, int8_t midi_note)
{   
    adsr_process(&o[voice][op].amp_adsr);
    adsr_process(&o[voice][op].pitch_asr);
    sine_osc_process_params(&o[voice][op].sine_osc, midi_note);
}

inline void operator_process_audio(uint8_t voice)
{
    for (int op = 0; op < SYNTH_OPERATORS_PER_VOICE; op++)
    {
        operator_t * x = &o[voice][op];

        sine_osc_process(&x->sine_osc, voice, op);
        vca_process(&x->vca, voice, op);

        audio_output_t temp = audio_get_src_phase(voice, x->params->src);
        temp = multiply_and_scale(temp, adsr_get_output(&x->amp_adsr), 16);
        audio_set_dst_phase(voice, op_dst[op], temp);
    }
    
}

void operator_init(uint8_t voice, uint8_t op, operator_params_t * params)
{

    operator_params_attach(voice, op, params);
    sine_osc_params_attach(voice, op, &p->sine_osc_params);
    vca_params_attach(voice, op, &p->vca_params);
    adsr_params_attach(voice, op, )
    
    operator_params_attach(o, params);
    sine_osc_params_attach(&o->sine_osc, &params->sine_osc_params);
    vca_params_attach(&o->vca, &params->vca_params);
    adsr_params_attach(&o->amp_adsr, &params->amp_adsr_params);
    adsr_params_attach(&o->pitch_asr, &params->pitch_asr_params);
}

void operator_start(operator_t * o)
{
    adsr_start(&o->amp_adsr);   
    adsr_start(&o->pitch_asr);
}