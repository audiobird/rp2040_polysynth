#ifndef __BUS_H__
#define __BUS_H__

#include <pico/stdio.h>
#include "signal.h"

typedef struct sum_bus_params
{
    const audio_input_t * a_in[2];
} sum_bus_params_t;

typedef struct sum_bus
{
    sum_bus_params_t * params;
    audio_output_t a_out;
} sum_bus_t;

typedef sum_bus_params_t ring_bus_params_t;

typedef struct ring_bus
{
    ring_bus_params_t * params;
    audio_output_t a_out;
} ring_bus_t;

void sum_bus_params_attach(sum_bus_t * bus, sum_bus_params_t * params);
void ring_bus_params_attach(ring_bus_t * bus, ring_bus_params_t * params);
void sum_bus_params_attach_audio_input(sum_bus_params_t * params, audio_output_t * src, bool channel);
void ring_bus_params_attach_audio_input(ring_bus_params_t * params, audio_output_t * src, bool channel);
audio_output_t * sum_bus_get_output(sum_bus_t * bus);
audio_output_t * ring_bus_get_output(ring_bus_t * ring);
void sum_bus_process_audio(sum_bus_t * bus);
void ring_bus_process_audio(ring_bus_t * ring);

void sum_bus_params_disable(sum_bus_params_t * params);
void ring_bus_params_disable(ring_bus_params_t * params);

#endif