#include "rate_reducer.h"

void rate_reducer_params_attach(rate_reducer_t * rr, rate_reducer_params_t * params)
{
    rr->params = params;
}

void rate_reducer_params_attach_audio_input(rate_reducer_params_t * params, audio_output_t * src)
{
    params->a_in = src;
}

void rate_reducer_params_set_amount(rate_reducer_params_t * params, int8_t amount)
{
    params->amount = amount;
}

audio_output_t * rate_reducer_get_output(rate_reducer_t * rr)
{
    return &rr->a_out;
}

void rate_reducer_process_audio(rate_reducer_t * rr)
{
    if (!rr->counter)
    {
        rr->a_out = *rr->params->a_in;
    }

    rr->counter += 1;

    if (rr->counter > rr->params->amount)
    rr->counter = 0;
}