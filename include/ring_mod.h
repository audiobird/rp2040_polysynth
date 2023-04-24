#ifndef __RINGMOD_H__
#define __RINGMOD_H__

#include <pico/stdio.h>
#include "signal.h"

typedef struct ring_mod_params
{
    const audio_input_t * a_in[2];
} ring_mod_params_t;

typedef struct ring_mod
{
    ring_mod_params_t * params;
    audio_output_t a_out;
} ring_mod_t;

void ring_mod_params_attach(ring_mod_t * ring_mod, ring_mod_params_t * params);
void ring_mod_params_attach_audio_input(ring_mod_params_t * params, audio_output_t * src, bool channel);
audio_output_t * ring_mod_get_output(ring_mod_t * ring_mod);
void ring_mod_process_audio(ring_mod_t * ring_mod);

void ring_mod_params_disable(ring_mod_params_t * params);

#endif