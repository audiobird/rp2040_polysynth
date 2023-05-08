#ifndef __RINGMOD_H__
#define __RINGMOD_H__

#include <pico/stdio.h>
#include "a_io.h"

typedef struct ring_mod_params
{
    signal_src_t src[2];
    bool enabled;
} ring_mod_params_t;

typedef struct ring_mod
{
    ring_mod_params_t * params;
} ring_mod_t;

void ring_mod_params_attach(ring_mod_t * ring_mod, ring_mod_params_t * params);
void ring_mod_process_audio(ring_mod_t * ring_mod, uint8_t voice);

void ring_mod_params_set_enable(ring_mod_params_t * params, bool enabled);


#endif