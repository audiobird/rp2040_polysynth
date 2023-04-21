#ifndef __RATE_REDUCER_H__
#define __RATE_RUDUCER_H__

#include <pico/stdio.h>
#include "signal.h"

typedef struct rate_reducer_params
{
    int8_t amount;
    audio_input_t * a_in;
} rate_reducer_params_t;

typedef struct rate_reducer
{
    uint8_t counter;
    rate_reducer_params_t * params;
    audio_output_t a_out;
} rate_reducer_t;

void rate_reducer_params_attach(rate_reducer_t * rr, rate_reducer_params_t * params);
void rate_reducer_params_attach_audio_input(rate_reducer_params_t * params, audio_output_t * src);
void rate_reducer_params_set_amount(rate_reducer_params_t * params, int8_t amount);
audio_output_t * rate_reducer_get_output(rate_reducer_t * rr);
void rate_reducer_process_audio(rate_reducer_t * rr);

#endif