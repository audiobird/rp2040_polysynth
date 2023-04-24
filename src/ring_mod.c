#include "ring_mod.h"
#include "common.h"

const audio_input_t ring_disable = 32767;

void ring_mod_params_attach(ring_mod_t * ring_mod, ring_mod_params_t * params)
{
    ring_mod->params = params;
}

void ring_mod_params_attach_audio_input(ring_mod_params_t * params, audio_output_t * src, bool channel)
{
    params->a_in[channel] = src;
}

audio_output_t * ring_mod_get_output(ring_mod_t * ring_mod)
{
    return &ring_mod->a_out;
}

void ring_mod_process_audio(ring_mod_t * ring_mod)
{
    int32_t a[2];

    a[0] = clamp(*ring_mod->params->a_in[0], INT16_MIN, INT16_MAX);
    a[1] = clamp(*ring_mod->params->a_in[1], INT16_MIN, INT16_MAX);

    ring_mod->a_out = multiply_and_scale(a[0], a[1], 15);
}

void ring_mod_params_disable(ring_mod_params_t * params)
{
    params->a_in[1] = &ring_disable;
}