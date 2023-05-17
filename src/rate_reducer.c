#include "rate_reducer.h"

static const signal_dst_t rr_dst = SRC_RR;

typedef struct rate_reducer
{
    uint8_t counter;
    rate_reducer_params_t * params;
} rate_reducer_t;

static rate_reducer_t rr[SYNTH_NUM_VOICES];

inline void rate_reducer_params_attach(uint8_t voice, rate_reducer_params_t * params)
{
    rr[voice].params = params;
}

inline void rate_reducer_params_set_amount(rate_reducer_params_t * params, int8_t m_val)
{
    params->amount = m_val;
}

inline void rate_reducer_process(uint8_t voice)
{
    if (!rr[voice].counter++)
    audio_set_dst_phase(voice, rr_dst, audio_get_src_phase(voice, rr[voice].params->src));

    if (rr[voice].counter > rr[voice].params->amount)
    rr[voice].counter = 0;
}