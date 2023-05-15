#include "voice.h"

const signal_dst_t voice_dst = SRC_VOICE;

voice_t v[SYNTH_NUM_VOICES];
voice_params_t params[SYNTH_NUM_TIMBRES] = 
{
    {
        //patch one
        .bc_params.src = SRC_OP0,
        .op_params[0].sine_osc_params.src = SRC_OP1,
        .op_params[0].src = SRC_VCA0,
        .op_params[0].vca_params.src = SRC_OSC0,
        .op_params[1].sine_osc_params.src = SRC_OSC1,
        .op_params[1].src = SRC_VCA1,
        .op_params[1].vca_params.src = SRC_OSC1,
        .rm_params.src[0] = SRC_RM,
        .rm_params.src[1] = SRC_RM,
        .rr_params.src = SRC_BC,
        .src = SRC_RR,
    },
    {
        //2
        .bc_params.src = SRC_OP1,
        .op_params[0].sine_osc_params.src = SRC_BC,
        .op_params[0].src = SRC_VCA0,
        .op_params[0].vca_params.src = SRC_OSC0,
        .op_params[1].sine_osc_params.src = SRC_RR,
        .op_params[1].src = SRC_VCA1,
        .op_params[1].vca_params.src = SRC_OSC1,
        .rm_params.src[0] = SRC_RM,
        .rm_params.src[1] = SRC_RM,
        .rr_params.src = SRC_OP0,
        .src = SRC_OP0,
    },
    {
        //3
        .bc_params.src = SRC_OP0,
        .op_params[0].sine_osc_params.src = SRC_RR,
        .op_params[0].src = SRC_VCA0,
        .op_params[0].vca_params.src = SRC_OSC0,
        .op_params[1].sine_osc_params.src = SRC_BC,
        .op_params[1].src = SRC_VCA1,
        .op_params[1].vca_params.src = SRC_RR,
        .rm_params.src[0] = SRC_BC,
        .rm_params.src[1] = SRC_RR,
        .rr_params.src = SRC_OSC1,
        .src = SRC_RM,
    },
    {
        //4
        .bc_params.src = SRC_OP0,
        .op_params[0].sine_osc_params.src = SRC_RM,
        .op_params[0].src = SRC_VCA0,
        .op_params[0].vca_params.src = SRC_OSC0,
        .op_params[1].sine_osc_params.src = SRC_RR,
        .op_params[1].src = SRC_VCA1,
        .op_params[1].vca_params.src = SRC_OSC1,
        .rm_params.src[0] = SRC_OSC0,
        .rm_params.src[1] = SRC_OP1,
        .rr_params.src = SRC_BC,
        .src = SRC_RR,
    },
};

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

enum cc_map 
{
    CC_ATTACK_OP_1,
    CC_DECAY_OP_1,
    CC_SUSTAIN_OP_1,
    CC_RELEASE_OP_1,
    CC_ADSR_EXP_OP_1,

    CC_LEVEL_OP_1,

    CC_OCTAVE_OP_1,
    CC_TRANSPOSE_OP_1,
    CC_FINE_TUNE_OP_1,
    CC_MOD_DEPTH_OP_1,

    CC_ATTACK_OP_2,
    CC_DECAY_OP_2,
    CC_SUSTAIN_OP_2,
    CC_RELEASE_OP_2,
    CC_ADSR_EXP_OP_2,

    CC_LEVEL_OP_2,

    CC_OCTAVE_OP_2,
    CC_TRANSPOSE_OP_2,
    CC_FINE_TUNE_OP_2,
    CC_MOD_DEPTH_OP_2,

    CC_RATE_REDUCE_AMOUNT,
    CC_BIT_CRUSH_AMOUNT,
    CC_RING_MOD_ENABLE,
};

void voice_handle_cc(uint8_t timbre, uint8_t controller, uint8_t value)
{
    voice_params_t * p = &params[timbre];

    switch (controller)
    {
        case CC_ATTACK_OP_1: 
        case CC_DECAY_OP_1:
        case CC_SUSTAIN_OP_1:
        case CC_RELEASE_OP_1:
        adsr_params_set(&p->op_params[0].amp_adsr_params, controller - CC_ATTACK_OP_1, value);
        break;

        case CC_ADSR_EXP_OP_1:
        adsr_params_set_exp(&p->op_params[0].amp_adsr_params, value);
        break;

        case CC_LEVEL_OP_1:
        vca_params_set_gain(&p->op_params[0].vca_params, value);
        break;

        case CC_OCTAVE_OP_1:
        sine_osc_params_set_octave_offset(&p->op_params[0].sine_osc_params, value);
        break;

        case CC_TRANSPOSE_OP_1:
        sine_osc_params_set_transpose(&p->op_params[0].sine_osc_params, value);
        break;

        case CC_FINE_TUNE_OP_1:
        sine_osc_params_set_fine_offset(&p->op_params[0].sine_osc_params, value);
        break;

        case CC_MOD_DEPTH_OP_1:
        sine_osc_params_set_mod_amount(&p->op_params[0].sine_osc_params, value);
        break;

        case CC_ATTACK_OP_2:
        case CC_DECAY_OP_2:
        case CC_SUSTAIN_OP_2:
        case CC_RELEASE_OP_2:
        adsr_params_set(&p->op_params[1].amp_adsr_params, controller - CC_ATTACK_OP_2, value);
        break;

        case CC_ADSR_EXP_OP_2:
        adsr_params_set_exp(&p->op_params[1].amp_adsr_params, value);
        break;

        case CC_LEVEL_OP_2:
        vca_params_set_gain(&p->op_params[1].vca_params, value);
        break;

        case CC_OCTAVE_OP_2:
        sine_osc_params_set_octave_offset(&p->op_params[1].sine_osc_params, value);
        break;

        case CC_TRANSPOSE_OP_2:
        sine_osc_params_set_transpose(&p->op_params[1].sine_osc_params, value);
        break;

        case CC_FINE_TUNE_OP_2:
        sine_osc_params_set_fine_offset(&p->op_params[1].sine_osc_params, value);
        break;

        case CC_MOD_DEPTH_OP_2:
        sine_osc_params_set_mod_amount(&p->op_params[1].sine_osc_params, value);
        break;

        case CC_RATE_REDUCE_AMOUNT:
        rate_reducer_params_set_amount(&p->rr_params, value);
        break;

        case CC_BIT_CRUSH_AMOUNT:
        bit_crusher_params_set_amount(&p->bc_params, value);
        break;

        case CC_RING_MOD_ENABLE:
        ring_mod_params_set_enable(&p->rm_params, value);
        break;
    }
}