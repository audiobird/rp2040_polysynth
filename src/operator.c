#include "operator.h"
#include "common.h"

void operator_params_attach(operator_t * operator, operator_params_t * params)
{
    operator->params = params;
    sine_osc_params_attach(&operator->sine_osc, &params->sine_osc_params);
    vca_params_attach(&operator->vca, &params->vca_params);
    adsr_params_attach(&operator->amp_adsr, &params->amp_adsr_params);
}

void operator_process_params(operator_t * op, int8_t midi_note)
{
    adsr_process(&op->amp_adsr);
    sine_osc_process_params(&op->sine_osc, midi_note);
}

void operator_process_audio(operator_t * op)
{
    sine_osc_process(&op->sine_osc);
    vca_process(&op->vca);
    audio_output_t temp = *vca_get_output(&op->vca);
    op->a_out = multiply_and_scale(temp, adsr_get_output(&op->amp_adsr), 16);
}

void operator_params_attach_audio_input(operator_params_t * params, audio_output_t * src)
{
    sine_osc_params_attach_audio_input(&params->sine_osc_params, src);
}

audio_output_t * operator_get_output(operator_t * op)
{
    return &op->a_out;
}

void operator_init(operator_t * operator, operator_params_t * params)
{
    operator_params_attach(operator, params);
    operator_params_attach_audio_input(operator->params, &operator->a_out);
    vca_params_attach_audio_input(operator->vca.params, sine_osc_get_output(&operator->sine_osc));
}