#include "bus.h"
#include "common.h"

const audio_input_t ring_disable = 32767;
const audio_input_t sum_disable = 0;

void sum_bus_params_attach(sum_bus_t * bus, sum_bus_params_t * params)
{
    bus->params = params;
}

void ring_bus_params_attach(ring_bus_t * bus, ring_bus_params_t * params)
{
    bus->params = params;
}

void sum_bus_params_attach_audio_input(sum_bus_params_t * params, audio_output_t * src, bool channel)
{
    params->a_in[channel] = src;
}

void ring_bus_params_attach_audio_input(ring_bus_params_t * params, audio_output_t * src, bool channel)
{
    params->a_in[channel] = src;
}

audio_output_t * sum_bus_get_output(sum_bus_t * bus)
{
    return &bus->a_out;
}

audio_output_t * ring_bus_get_output(ring_bus_t * ring)
{
    return &ring->a_out;
}


void sum_bus_process_audio(sum_bus_t * bus)
{
    bus->a_out = *bus->params->a_in[0] + *bus->params->a_in[1];
}

void ring_bus_process_audio(ring_bus_t * ring)
{
    int32_t a[2];

    a[0] = clamp(*ring->params->a_in[0], INT16_MIN, INT16_MAX);
    a[1] = clamp(*ring->params->a_in[1], INT16_MIN, INT16_MAX);

    ring->a_out = multiply_and_scale(a[0], a[1], 15);
}

void sum_bus_params_disable(sum_bus_params_t * params)
{
    params->a_in[1] = &sum_disable;
}

void ring_bus_params_disable(ring_bus_params_t * params)
{
    params->a_in[1] = &ring_disable;
}