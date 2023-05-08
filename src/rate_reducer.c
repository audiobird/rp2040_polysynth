#include "rate_reducer.h"

const signal_dst_t rr_dst = SRC_RR;

void rate_reducer_params_attach(rate_reducer_t * rr, rate_reducer_params_t * params)
{
    rr->params = params;
}

void rate_reducer_params_set_amount(rate_reducer_params_t * params, int8_t m_val)
{
    params->amount = m_val;
}

void rate_reducer_process_audio(rate_reducer_t * rr, uint8_t voice)
{
    if (!rr->counter)
    {
        audio_set_dst_phase(voice, rr_dst, audio_get_src_phase(voice, rr->params->src));
    }

    rr->counter += 1;

    if (rr->counter > rr->params->amount)
    rr->counter = 0;
}