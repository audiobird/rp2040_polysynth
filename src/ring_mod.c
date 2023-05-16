#include "ring_mod.h"
#include "common.h"

static const int16_t ring_disable = 32767;
static const signal_dst_t rm_dst = SRC_RM;

typedef struct ring_mod
{
    ring_mod_params_t * params;
} ring_mod_t;

static ring_mod_t rm[SYNTH_NUM_VOICES];

inline void ring_mod_params_attach(uint8_t voice, ring_mod_params_t * params)
{
    rm[voice].params = params;
}

inline void ring_mod_process_audio(uint8_t voice)
{
    int32_t a[2];

    if (rm[voice].params->enabled)
    {
        a[0] = audio_get_src_phase(voice, rm[voice].params->src[0]);
        a[1] = audio_get_src_phase(voice, rm[voice].params->src[1]);
        audio_set_dst_phase(voice, rm_dst, multiply_and_scale(a[0], a[1], 15));
        return;
    }
    
    a[0] = audio_get_src_phase(voice, rm[voice].params->src[0]);
    a[1] = audio_get_src_phase(voice, rm[voice].params->src[1]);
    
    audio_set_dst_phase(voice, rm_dst, audio_sum_and_clamp(a[0], a[1]));    
}

inline void ring_mod_params_set_enable(ring_mod_params_t * params, uint8_t m_val)
{
    params->enabled = midi_to_switch(m_val);
}