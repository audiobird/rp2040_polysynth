#include "voice.h"

const signal_dst_t voice_dst = SRC_VOICE;

void voice_process_params(voice_t * v)
{
    for (int op = 0; op < SYNTH_OPERATORS_PER_VOICE; op++)
    {
        operator_process_params(&v->operator[op], v->midi_note);
    }
}

void voice_process_audio(voice_t * v, uint8_t voice)
{
    for (int op = 0; op < SYNTH_OPERATORS_PER_VOICE; op++)
    {
        operator_process_audio(&v->operator[op], voice, op);
    }
    bit_crusher_process_audio(&v->bit_crusher, voice);
    rate_reducer_process_audio(&v->rate_reducer, voice);
    ring_mod_process_audio(&v->ring_mod, voice);
    audio_set_dst_phase(voice, voice_dst, audio_get_src_phase(voice, v->params->src));
}

audio_output_t voice_get_all()
{
    audio_output_t o;

    o  = audio_get_src_phase(0, SRC_VOICE);
    o += audio_get_src_phase(1, SRC_VOICE);
    o += audio_get_src_phase(2, SRC_VOICE);
    o += audio_get_src_phase(3, SRC_VOICE);

    return o;
}

void voice_set_gate(voice_t * v, gate_t state)
{
    v->gate = state;
}

void voice_set_midi_note(voice_t * v, int8_t midi_note)
{
    v->midi_note = midi_note;
}

void voice_attach_params(voice_t * v, voice_params_t * params)
{
    v->params = params;
}

void voice_params_setup_algorithm(voice_params_t * params, uint8_t alg)
{
    switch (alg)
    {
        case 0:
        {
            //sequential fm with rate reducer and bit crusher on the tail
            //feedback on second operator
            params->src = SRC_RR;
            params->rr_params.src = SRC_BC;
            params->bc_params.src = SRC_OP0;
            params->op_params[0].src = SRC_VCA0;
            params->op_params[0].vca_params.src = SRC_OSC0;
            params->op_params[0].sine_osc_params.src = SRC_OP1;
            params->op_params[1].src = SRC_VCA1; 
            params->op_params[1].vca_params.src = SRC_OSC1;
            params->op_params[1].sine_osc_params.src = SRC_OSC1; //fb
            params->rm_params.src[0] = SRC_RM;
            params->rm_params.src[1] = SRC_RM;
            break;
        }
        case 1:
        {
            //sequential fm with rate reducer in feedback path, bit crusher in between operators
            //feedback goes from 1st op to 2nd op
            params->src = SRC_OP0;
            params->op_params[0].src = SRC_VCA0;
            params->op_params[0].vca_params.src = SRC_OSC0;
            params->op_params[0].sine_osc_params.src = SRC_BC;
            params->bc_params.src = SRC_OP1;
            params->op_params[1].src = SRC_VCA1;
            params->op_params[1].vca_params.src = SRC_OSC0;
            params->op_params[1].sine_osc_params.src = SRC_RR;
            params->rr_params.src = SRC_OP0;
            params->rm_params.src[0] = SRC_RM;
            params->rm_params.src[1] = SRC_RM;
            break;
        }
        case 2:
        {
            //duophonic voice each with a lofi effect modulating eachother, and ring modded together
            params->src = SRC_RM;
            params->rm_params.src[0] = SRC_BC;
            params->bc_params.src = SRC_OP0;
            params->op_params[0].src = SRC_VCA0;
            params->op_params[0].vca_params.src = SRC_OSC0;
            params->op_params[0].sine_osc_params.src = SRC_RR;
            params->rm_params.src[1] = SRC_RR;
            params->op_params[1].src = SRC_VCA1;
            params->op_params[1].vca_params.src = SRC_RR;
            params->rr_params.src = SRC_OSC1;
            params->op_params[1].sine_osc_params.src = SRC_BC;
            break;
        }
        case 3:
        {
            //operator 2 and operator 1 are ring modded and fed to the feedback path of op 1
            //bit crusher and rate reducer on output.
            params->src = SRC_RR;
            params->rr_params.src = SRC_BC;
            params->bc_params.src = SRC_OP0;
            params->op_params[0].src = SRC_VCA0;
            params->op_params[0].vca_params.src = SRC_OSC0;
            params->op_params[0].sine_osc_params.src = SRC_RM;
            params->rm_params.src[0] = SRC_OSC0;
            params->rm_params.src[1] = SRC_OP1;
            params->op_params[1].src = SRC_VCA1; 
            params->op_params[1].vca_params.src = SRC_OSC1;
            params->op_params[1].sine_osc_params.src = SRC_RR; //gnarly.
            break;
        }

    }
}

void voice_init(voice_t * v, voice_params_t * params, uint8_t alg)
{
    voice_attach_params(v, params);

    for (int op = 0; op < SYNTH_OPERATORS_PER_VOICE; op++)
    {
        operator_init(&v->operator[op], &params->op_params[op]);
        sine_osc_attach_gate(&v->operator[op].sine_osc, &v->gate);
        adsr_attach_gate(&v->operator[op].amp_adsr, &v->gate);
    }
    

    bit_crusher_params_attach(&v->bit_crusher, &params->bc_params);
    rate_reducer_params_attach(&v->rate_reducer, &params->rr_params);
    ring_mod_params_attach(&v->ring_mod, &params->rm_params);

    voice_params_setup_algorithm(params, alg);
}
