#include "bit_crusher.h"
#include "common.h"

const signal_dst_t bc_dst = SRC_BC;

void bit_crusher_params_attach(bit_crusher_t * bc, bit_crusher_params_t * params)
{
    bc->params = params;
}

void bit_crusher_params_set_amount(bit_crusher_params_t * params, int8_t m_val)
{
    params->mask = ~((0 << 15) | (m_val << 8) | (m_val << 1) | (m_val >> 6));
}

void bit_crusher_process_audio(bit_crusher_t * bc, uint8_t voice)
{
    audio_input_t a = audio_get_src_phase(voice, bc->params->src);

    a &= bc->params->mask;

    audio_set_dst_phase(voice, bc_dst, a);
}
