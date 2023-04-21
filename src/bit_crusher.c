#include "bit_crusher.h"
#include "common.h"

void bit_crusher_params_attach(bit_crusher_t * bc, bit_crusher_params_t * params)
{
    bc->params = params;
}

void bit_crusher_params_attach_audio_input(bit_crusher_params_t * params, audio_output_t * src)
{
    params->a_in = src;
}

void bit_crusher_params_set_amount(bit_crusher_params_t * params, int8_t amount)
{
    params->amount = clamp(amount, 0, 15);
}

audio_output_t * bit_crusher_get_output(bit_crusher_t * bc)
{
    return &bc->a_out;
}

void bit_crusher_process_audio(bit_crusher_t * bc)
{
    int32_t a = *bc->params->a_in;

    a &= 0xfffffffful << bc->params->amount;

    bc->a_out = a;
}