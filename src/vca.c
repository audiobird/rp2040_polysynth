#include "vca.h"
#include "common.h"
#include "tables/exp_table.h"

const signal_dst_t vca_dst[SYNTH_OPERATORS_PER_VOICE] = {SRC_VCA0, SRC_VCA1};

void vca_params_set_gain(vca_params_t * params, int8_t m_val)
{
    uint16_t x = m_val << 9;
    x |= m_val << 2;
    x |= (uint)(m_val) >> 5;
    params->gain = exp_table[x];
}

void vca_process(vca_t * vca, uint8_t voice, uint8_t op)
{
    audio_input_t x = audio_get_src_phase(voice, vca->params->src);
    x = multiply_and_scale(x, vca->params->gain, 16);
    audio_set_dst_phase(voice, vca_dst[op], x);
}

void vca_params_attach(vca_t * vca, vca_params_t * params)
{
    vca->params = params;
}
