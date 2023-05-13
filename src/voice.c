#include "voice.h"

const signal_dst_t voice_dst = SRC_VOICE;

voice_t v[SYNTH_NUM_VOICES];
voice_params_t params[SYNTH_NUM_TIMBRES];

void voice_note_on(uint8_t voice, uint8_t timbre, uint8_t note, uint8_t velocity)
{
    v[voice].timbre = timbre;
    v[voice].midi_note = note;
    v[voice].velocity = velocity;
    v[voice].gate = 1;
}

void voice_note_off(uint8_t voice)
{
    v[voice].gate = 0;
}

void voice_main(uint8_t voice)
{
    bool prev_gate[SYNTH_NUM_VOICES] = {1};

    voice_t* x = &v[voice];

    if(x->gate == prev_gate[voice])
    return;

    prev_gate[voice] = x->gate;

    if (x->gate)
    {
        voice_attach_params(voice, x->timbre);
        operator_start(&x->operator[0]);
        operator_start(&x->operator[1]);
    }
    else
    {
        adsr_release(&x->operator[0].amp_adsr);
        adsr_release(&x->operator[1].amp_adsr); 
    }
}

void voice_process_params(uint8_t voice)
{
    for (int op = 0; op < SYNTH_OPERATORS_PER_VOICE; op++)
    {
        operator_process_params(&v[voice].operator[op], v[voice].midi_note);
    }
}

void voice_process_audio(uint8_t voice)
{
    for (int op = 0; op < SYNTH_OPERATORS_PER_VOICE; op++)
    {
        operator_process_audio(&v[voice].operator[op], voice, op);
    }
    bit_crusher_process_audio(&v[voice].bit_crusher, voice);
    rate_reducer_process_audio(&v[voice].rate_reducer, voice);
    ring_mod_process_audio(&v[voice].ring_mod, voice);
    audio_set_dst_phase(voice, voice_dst, audio_get_src_phase(voice, v[voice].params->src));
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

void voice_attach_params(uint8_t voice, uint8_t timbre)
{
    v[voice].params = &params[timbre];
}

void voice_params_setup_algorithm(uint8_t timbre, uint8_t alg)
{
    switch (alg)
    {
        case 0:
        {
            //sequential fm with rate reducer and bit crusher on the tail
            //feedback on second operator
            params[timbre].src = SRC_RR;
            params[timbre].rr_params.src = SRC_BC;
            params[timbre].bc_params.src = SRC_OP0;
            params[timbre].op_params[0].src = SRC_VCA0;
            params[timbre].op_params[0].vca_params.src = SRC_OSC0;
            params[timbre].op_params[0].sine_osc_params.src = SRC_OP1;
            params[timbre].op_params[1].src = SRC_VCA1; 
            params[timbre].op_params[1].vca_params.src = SRC_OSC1;
            params[timbre].op_params[1].sine_osc_params.src = SRC_OSC1; //fb
            params[timbre].rm_params.src[0] = SRC_RM;
            params[timbre].rm_params.src[1] = SRC_RM;
            break;
        }
        case 1:
        {
            //sequential fm with rate reducer in feedback path, bit crusher in between operators
            //feedback goes from 1st op to 2nd op
            params[timbre].src = SRC_OP0;
            params[timbre].op_params[0].src = SRC_VCA0;
            params[timbre].op_params[0].vca_params.src = SRC_OSC0;
            params[timbre].op_params[0].sine_osc_params.src = SRC_BC;
            params[timbre].bc_params.src = SRC_OP1;
            params[timbre].op_params[1].src = SRC_VCA1;
            params[timbre].op_params[1].vca_params.src = SRC_OSC1;
            params[timbre].op_params[1].sine_osc_params.src = SRC_RR;
            params[timbre].rr_params.src = SRC_OP0;
            params[timbre].rm_params.src[0] = SRC_RM;
            params[timbre].rm_params.src[1] = SRC_RM;
            break;
        }
        case 2:
        {
            //duophonic voice each with a lofi effect modulating eachother, and ring modded together
            params[timbre].src = SRC_RM;
            params[timbre].rm_params.src[0] = SRC_BC;
            params[timbre].bc_params.src = SRC_OP0;
            params[timbre].op_params[0].src = SRC_VCA0;
            params[timbre].op_params[0].vca_params.src = SRC_OSC0;
            params[timbre].op_params[0].sine_osc_params.src = SRC_RR;
            params[timbre].rm_params.src[1] = SRC_RR;
            params[timbre].op_params[1].src = SRC_VCA1;
            params[timbre].op_params[1].vca_params.src = SRC_RR;
            params[timbre].rr_params.src = SRC_OSC1;
            params[timbre].op_params[1].sine_osc_params.src = SRC_BC;
            break;
        }
        case 3:
        {
            //operator 2 and operator 1 are ring modded and fed to the feedback path of op 1
            //bit crusher and rate reducer on output.
            params[timbre].src = SRC_RR;
            params[timbre].rr_params.src = SRC_BC;
            params[timbre].bc_params.src = SRC_OP0;
            params[timbre].op_params[0].src = SRC_VCA0;
            params[timbre].op_params[0].vca_params.src = SRC_OSC0;
            params[timbre].op_params[0].sine_osc_params.src = SRC_RM;
            params[timbre].rm_params.src[0] = SRC_OSC0;
            params[timbre].rm_params.src[1] = SRC_OP1;
            params[timbre].op_params[1].src = SRC_VCA1; 
            params[timbre].op_params[1].vca_params.src = SRC_OSC1;
            params[timbre].op_params[1].sine_osc_params.src = SRC_RR; //gnarly.
            break;
        }

    }
}

void voice_init(uint8_t voice, uint8_t timbre, uint8_t alg)
{
    voice_attach_params(voice, timbre);

    for (int op = 0; op < SYNTH_OPERATORS_PER_VOICE; op++)
    {
        operator_init(&v[voice].operator[op], &params[timbre].op_params[op]);
    }
    

    bit_crusher_params_attach(&v[voice].bit_crusher, &params[timbre].bc_params);
    rate_reducer_params_attach(&v[voice].rate_reducer, &params[timbre].rr_params);
    ring_mod_params_attach(&v[voice].ring_mod, &params[timbre].rm_params);

    voice_params_setup_algorithm(timbre, alg);

    adsr_params_set(&params[0].op_params[0].amp_adsr_params, ADSR_P_A, 16);
    adsr_params_set(&params[0].op_params[0].amp_adsr_params, ADSR_P_D, 8);
    adsr_params_set(&params[0].op_params[0].amp_adsr_params, ADSR_P_S, 64);
    adsr_params_set(&params[0].op_params[0].amp_adsr_params, ADSR_P_R, 4);

    adsr_params_set(&params[0].op_params[1].amp_adsr_params, ADSR_P_A, 127);
    adsr_params_set(&params[0].op_params[1].amp_adsr_params, ADSR_P_D, 3);
    adsr_params_set(&params[0].op_params[1].amp_adsr_params, ADSR_P_S, 32);
    adsr_params_set(&params[0].op_params[1].amp_adsr_params, ADSR_P_R, 16);

    params[0].op_params[0].vca_params.gain = 65535;
    
}

void voice_handle_cc(uint8_t timbre, uint8_t controller, uint8_t value)
{
    switch (controller)
    {
        case 0: vca_params_set_gain(&params[timbre].op_params[1].vca_params, value); break;
        case 1: bit_crusher_params_set_amount(&params[timbre].bc_params, value); break;
        case 2: rate_reducer_params_set_amount(&params[timbre].rr_params, value); break;
        case 3: sine_osc_params_set_mod_amount(&params[timbre].op_params[0].sine_osc_params, value); break;
        case 4: sine_osc_params_set_mod_amount(&params[timbre].op_params[1].sine_osc_params, value); break;
        case 5: sine_osc_params_set_transpose(&params[timbre].op_params[1].sine_osc_params, value); break;
        case 6: break;
        case 7: break;
    }
}