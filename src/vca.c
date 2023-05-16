#include "vca.h"
#include "common.h"
#include "tables/exp_table.h"

static const signal_dst_t vca_dst[SYNTH_OPERATORS_PER_VOICE] = {SRC_VCA0, SRC_VCA1};

static vca_t v[SYNTH_NUM_VOICES][SYNTH_OPERATORS_PER_VOICE];

inline void vca_params_set_gain(vca_params_t * params, int8_t m_val)
{
    uint16_t x = m_val << 9;
    x |= m_val << 2;
    x |= (uint)(m_val) >> 5;
    params->gain = exp_table[x];
}

inline void vca_process(uint8_t voice, uint8_t op)
{
    audio_input_t x = audio_get_src_phase(voice, v[voice][op].params->src);
    x = multiply_and_scale(x, v[voice][op].params->gain, 16);
    audio_set_dst_phase(voice, vca_dst[op], x);
}

inline void vca_params_attach(uint8_t voice, uint8_t op, vca_params_t * params)
{
    v[voice][op].params = params;
}
