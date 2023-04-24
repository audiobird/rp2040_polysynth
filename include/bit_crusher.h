#ifndef __BIT_CRUSHER_H__
#define __BIT_CRUSHER_H__

#include <pico/stdio.h>
#include "signal.h"

//this could definitely all be improved.

typedef struct bit_crusher_params
{
    int8_t amount;
    audio_input_t * a_in;
} bit_crusher_params_t;

typedef struct bit_crusher
{
    bit_crusher_params_t * params;
    audio_io_t audio;
} bit_crusher_t;

void bit_crusher_params_attach(bit_crusher_t * bc, bit_crusher_params_t * params);
void bit_crusher_attach_audio_input(bit_crusher_t * bc, audio_io_t * io);
void bit_crusher_params_set_amount(bit_crusher_params_t * params, int8_t amount);
audio_io_t * bit_crusher_get_io(bit_crusher_t * bc);
void bit_crusher_process_audio(bit_crusher_t * bc);


#endif