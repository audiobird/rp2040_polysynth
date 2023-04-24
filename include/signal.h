#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#include <pico/stdio.h>

typedef int32_t audio_input_t;
typedef int32_t audio_output_t;

typedef struct audio_io
{
    audio_input_t * in;
    audio_output_t out;
} audio_io_t;

typedef int32_t cv_input_t;
typedef int32_t cv_output_t;

#endif