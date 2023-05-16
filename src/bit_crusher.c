#include "bit_crusher.h"
#include "common.h"

static const signal_dst_t bc_dst = SRC_BC;

typedef struct bit_crusher
{
    bit_crusher_params_t * params;
} bit_crusher_t;

static bit_crusher_t bc[SYNTH_NUM_VOICES];

inline void bit_crusher_params_attach(uint8_t voice, bit_crusher_params_t * params)
{
    bc[voice].params = params;
}

inline void bit_crusher_params_set_amount(bit_crusher_params_t * params, int8_t m_val)
{
    params->mask = ~((0 << 15) | (m_val << 8) | (m_val << 1) | (m_val >> 6));
}

inline void bit_crusher_process_audio(uint8_t voice)
{
    audio_input_t a = audio_get_src_phase(voice, bc[voice].params->src);

    a &= (0xffff0000) | bc[voice].params->mask;

    audio_set_dst_phase(voice, bc_dst, a);
}
