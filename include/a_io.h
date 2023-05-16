#ifndef __A_IO_H__
#define __A_IO_H__

#include <pico/stdio.h>

typedef enum 
{
    SRC_OSC0,
    SRC_OSC1,
    SRC_VCA0,
    SRC_VCA1,
    SRC_ADSR0,
    SRC_ADSR1,
    SRC_RM,
    SRC_BC,
    SRC_RR,
    SRC_CNT,
} signal_src_t;

typedef signal_src_t signal_dst_t;

typedef int32_t audio_input_t;
typedef int32_t audio_output_t;

typedef struct audio_io
{
    audio_input_t * in;
    audio_output_t out;
} audio_io_t;

void audio_connect_src_to_sink(uint8_t voice, signal_src_t src, audio_output_t * dst);
audio_input_t audio_get_src_phase(uint8_t voice, signal_src_t src);
void audio_set_dst_phase(uint8_t voice, signal_dst_t dst, audio_output_t phase);

#endif

/*

?
osc
vca
op

*/