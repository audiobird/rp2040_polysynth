#ifndef __RINGMOD_H__
#define __RINGMOD_H__

#include <pico/stdio.h>
#include "a_io.h"

typedef struct ring_mod_params
{
    signal_src_t src[2];
    
    bool enabled;
} ring_mod_params_t;

void ring_mod_params_attach(uint8_t voice, ring_mod_params_t * params);
void ring_mod_process_audio(uint8_t voice);

void ring_mod_params_set_enable(ring_mod_params_t * params, uint8_t m_val);

#endif