#ifndef __RATE_REDUCER_H__
#define __RATE_RUDUCER_H__

#include <pico/stdio.h>
#include "a_io.h"

typedef struct rate_reducer_params
{
    int8_t amount;
    signal_src_t src;
} rate_reducer_params_t;

void rate_reducer_params_attach(uint8_t voice, rate_reducer_params_t * params);
void rate_reducer_params_set_amount(rate_reducer_params_t * params, int8_t m_val);
void rate_reducer_process(uint8_t voice);

#endif