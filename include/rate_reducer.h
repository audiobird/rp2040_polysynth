#ifndef __RATE_REDUCER_H__
#define __RATE_RUDUCER_H__

#include <pico/stdio.h>
#include "a_io.h"

typedef struct rate_reducer_params
{
    int8_t amount;
    signal_src_t src;
} rate_reducer_params_t;

typedef struct rate_reducer
{
    uint8_t counter;
    rate_reducer_params_t * params;
    audio_output_t a_out;
} rate_reducer_t;

void rate_reducer_params_attach(rate_reducer_t * rr, rate_reducer_params_t * params);
void rate_reducer_params_set_amount(rate_reducer_params_t * params, int8_t m_val);
void rate_reducer_process_audio(rate_reducer_t * rr, uint8_t voice);

#endif