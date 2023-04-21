#include "voice.h"

audio_output_t * voice_get_output(voice_t * voice)
{
    return &voice->a_out;
}

void voice_process_params(voice_t * voice)
{
    for (int x = 0; x < SYNTH_OPERATORS_PER_VOICE; x++)
    {
        operator_process_params(&voice->operator[x], voice->midi_note);
    }
}

void voice_process_audio(voice_t * voice)
{
    for (int x = 0; x < SYNTH_OPERATORS_PER_VOICE; x++)
    {
        operator_process_audio(&voice->operator[x]);
    }
    bit_crusher_process_audio(&voice->bit_crusher);
    rate_reducer_process_audio(&voice->rate_reducer);
    sum_bus_process_audio(&voice->sum_bus);
    ring_bus_process_audio(&voice->ring_bus);
    voice->a_out = *operator_get_output(&voice->operator[0]);
}

void voice_set_gate(voice_t * voice, gate_t state)
{
    voice->gate = state;
}

void voice_set_midi_note(voice_t * voice, int8_t midi_note)
{
    voice->midi_note = midi_note;
}

void voice_attach_params(voice_t * voice, voice_params_t * params)
{
    voice->params = params;
}

void voice_init(voice_t * voice, voice_params_t * params, uint8_t alg)
{
    assert(alg < 16);

    voice->params = params;

    for (int x = 0; x < SYNTH_OPERATORS_PER_VOICE; x++)
    {
        operator_init(&voice->operator[x], &params->op_params[x]);
        sine_osc_attach_gate(&voice->operator[x].sine_osc, &voice->gate);
        adsr_attach_gate(&voice->operator[x].amp_adsr, &voice->gate);
    }

    bit_crusher_params_attach(&voice->bit_crusher, &params->bc_params);
    rate_reducer_params_attach(&voice->rate_reducer, &params->rr_params);
    sum_bus_params_attach(&voice->sum_bus, &params->sb_params);
    ring_bus_params_attach(&voice->ring_bus, &params->rb_params);

    switch (alg)
    {
        case 0:
        {
            //3 -> bc -fb> 2 -> rr -> 1 -> rm(dis) -> 0 -> sum(dis) -> out
            bit_crusher_params_attach_audio_input(&params->bc_params, operator_get_output(&voice->operator[3]));
            operator_params_attach_audio_input(&params->op_params[3], bit_crusher_get_output(&voice->bit_crusher));
            operator_params_attach_audio_input(&params->op_params[2], bit_crusher_get_output(&voice->bit_crusher));
            rate_reducer_params_attach_audio_input(&params->rr_params, operator_get_output(&voice->operator[2]));
            operator_params_attach_audio_input(&params->op_params[1], rate_reducer_get_output(&voice->rate_reducer));
            ring_bus_params_attach_audio_input(&params->rb_params, operator_get_output(&voice->operator[1]), 0);
            ring_bus_params_disable(&params->rb_params);
            operator_params_attach_audio_input(&params->op_params[0], ring_bus_get_output(&voice->ring_bus));
            sum_bus_params_attach_audio_input(&params->sb_params, operator_get_output(&voice->operator[0]), 0);
            sum_bus_params_disable(&params->sb_params);
        }
    }
}