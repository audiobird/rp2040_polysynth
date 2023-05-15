#include "ring_mod.h"
#include "common.h"

const int16_t ring_disable = 32767;
const signal_dst_t rm_dst = SRC_RM;

void ring_mod_params_attach(ring_mod_t * ring_mod, ring_mod_params_t * params)
{
    ring_mod->params = params;
}

void ring_mod_process_audio(ring_mod_t * ring_mod, uint8_t voice)
{
    int32_t a[2];

    if (ring_mod->params->enabled)
    {
        a[0] = clamp(audio_get_src_phase(voice, ring_mod->params->src[0]), INT16_MIN, INT16_MAX);
        a[1] = clamp(audio_get_src_phase(voice, ring_mod->params->src[1]), INT16_MIN, INT16_MAX);
        audio_set_dst_phase(voice, rm_dst, multiply_and_scale(a[0], a[1], 15));
        return;
    }
    
    a[0] = audio_get_src_phase(voice, ring_mod->params->src[0]);
    a[1] = audio_get_src_phase(voice, ring_mod->params->src[1]);
    
    audio_set_dst_phase(voice, rm_dst, audio_sum_and_clamp(a[0], a[1]));    
}

void ring_mod_params_set_enable(ring_mod_params_t * params, uint8_t m_val)
{
    params->enabled = m_val >= 64;
}