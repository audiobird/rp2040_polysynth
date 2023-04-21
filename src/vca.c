#include "vca.h"
#include "common.h"

void vca_params_set_gain(vca_params_t * params, int8_t gain)
{
    params->gain = gain;
}

void vca_params_attach_cv_input(vca_params_t * params, cv_output_t * src)
{
    params->cv_in = src;
}

void vca_process(vca_t * vca)
{
    int32_t x = *vca->params->a_in;
    x = multiply_and_scale(x, vca->params->gain, 7);
    vca->a_out = x;//multiply_and_scale(x, *vca->params->cv_in, 15);
}

void vca_params_attach(vca_t * vca, vca_params_t * params)
{
    vca->params = params;
}

void vca_params_attach_audio_input(vca_params_t * params, audio_output_t * src)
{
    params->a_in = src;
}

audio_output_t * vca_get_output(vca_t * vca)
{
    return &vca->a_out;
}