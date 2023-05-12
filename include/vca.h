#ifndef __VCA_H__
#define __VCA_H__

#include <pico/stdio.h>
#include "adsr.h"
#include "a_io.h"

typedef struct vca_params
{
    uint16_t gain;
    signal_src_t src;
} vca_params_t;

typedef struct vca
{
    vca_params_t * params;
} vca_t;

void vca_params_set_gain(vca_params_t * params, int8_t m_val);
void vca_params_attach_audio_input(vca_params_t * params, audio_output_t * src);
void vca_params_attach(vca_t * vca, vca_params_t * params);
void vca_process(vca_t * vca, uint8_t voice, uint8_t op);

#endif