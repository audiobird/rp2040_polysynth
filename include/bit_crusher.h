#ifndef __BIT_CRUSHER_H__
#define __BIT_CRUSHER_H__

#include <pico/stdio.h>
#include "a_io.h"

//this could definitely all be improved.

typedef struct bit_crusher_params
{
    uint16_t mask;
    signal_src_t src;
} bit_crusher_params_t;

typedef struct bit_crusher
{
    bit_crusher_params_t * params;
} bit_crusher_t;

void bit_crusher_params_attach(bit_crusher_t * bc, bit_crusher_params_t * params);
void bit_crusher_params_set_amount(bit_crusher_params_t * params, int8_t m_val);
void bit_crusher_process_audio(bit_crusher_t * bc, uint8_t voice);


#endif