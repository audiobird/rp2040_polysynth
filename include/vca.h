#ifndef __VCA_H__
#define __VCA_H__

#include <pico/stdio.h>
#include "adsr.h"
#include "signal.h"

typedef struct vca_params
{
    audio_input_t * a_in;
    cv_input_t * cv_in;
    int8_t gain;
} vca_params_t;

typedef struct vca
{
    audio_output_t a_out;
    vca_params_t * params;
} vca_t;

void vca_params_set_gain(vca_params_t * params, int8_t gain);
void vca_params_attach_audio_input(vca_params_t * params, audio_output_t * src);
void vca_params_attach_cv_input(vca_params_t * params, cv_output_t * src);
void vca_params_attach(vca_t * vca, vca_params_t * params);
void vca_process(vca_t * vca);
audio_output_t * vca_get_output(vca_t * vca);

#endif