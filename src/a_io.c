#include "a_io.h"
#include <string.h>

audio_output_t aio[SYNTH_NUM_VOICES][SRC_CNT];

audio_input_t audio_get_src_phase(uint8_t voice, signal_src_t src)
{
    return aio[voice][src];
}

void audio_set_dst_phase(uint8_t voice, signal_dst_t dst, audio_output_t phase)
{
    aio[voice][dst] = phase;
}

void aio_clear_busses(uint8_t voice)
{
    memset(aio[voice], 0, sizeof(aio[0]));
}