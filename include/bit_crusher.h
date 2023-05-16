#ifndef __BIT_CRUSHER_H__
#define __BIT_CRUSHER_H__

#include <pico/stdio.h>
#include "a_io.h"

typedef struct bit_crusher_params
{
    uint16_t mask;
    signal_src_t src;
} bit_crusher_params_t;

void bit_crusher_params_attach(uint8_t voice, bit_crusher_params_t * params);
void bit_crusher_params_set_amount(bit_crusher_params_t * params, int8_t m_val);
void bit_crusher_process_audio(uint8_t voice);

#endif