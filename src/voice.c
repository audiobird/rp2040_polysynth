#include "voice.h"

typedef struct voice
{
    voice_params_t * params;
    uint8_t timbre;
    uint8_t velocity;
    int16_t pitch_bend;
    int8_t midi_note;
    bool gate;
    audio_output_t out;
} voice_t;

static voice_t v[SYNTH_NUM_VOICES];

voice_params_t params[SYNTH_NUM_TIMBRES] = 
{
    {
        .src = SRC_BC,
        .bc_p.src = SRC_RR,
        .rr_p.src = SRC_VCA0,
        .vca_p[0].src = SRC_ADSR0,
        .adsr_p[0][ADSR_TYPE_AMP].src = SRC_OSC0,
        .osc_p[0].src = SRC_VCA1,
        .vca_p[1].src = SRC_ADSR1,
        .adsr_p[1][ADSR_TYPE_AMP].src = SRC_OSC1,
        .osc_p[1].src = SRC_OSC1,
        .rm_p.src[0] = SRC_RM,
        .rm_p.src[1] = SRC_RM,
        
        .adsr_p[0 ... 1][0 ... 1].a = 1024,
        .adsr_p[0 ... 1][0 ... 1].d = 32,
        .adsr_p[0 ... 1][0 ... 1].s = 48800,
        .adsr_p[0 ... 1][0 ... 1].r = 64,
        .adsr_p[0 ... 1][0 ... 1].exp = 1,
        .osc_p[0 ... 1].track_pitch = 1,
        .vca_p[0 ... 1].gain = 65535,
        .bc_p.mask = 0xffff,
    },
};

inline void voice_note_on(uint8_t voice, uint8_t timbre, uint8_t note, uint8_t velocity)
{
    v[voice].timbre = timbre;
    v[voice].midi_note = note;
    v[voice].velocity = velocity;
    v[voice].gate = 1;
}

inline void voice_note_off(uint8_t voice)
{
    v[voice].gate = 0;
}

inline void voice_process_params(uint8_t voice)
{
    static bool prev_gate[SYNTH_NUM_VOICES] = {1};

    voice_t* x = &v[voice];

    if(x->gate == prev_gate[voice])
    return;

    prev_gate[voice] = x->gate;

    if (x->gate)
    {
        voice_attach_params(voice, x->timbre);
        adsr_trig_voice(voice);
        sine_osc_trig_voice(voice, x->midi_note);
    }
    else
    {
        adsr_release_voice(voice);
    }

    for (int op = 0; op < SYNTH_OPERATORS_PER_VOICE; op++)
    {
        for (int adsr = 0; adsr < ADSR_TYPE_CNT; adsr++)
        {
            adsr_process_envelope(voice, op, adsr);
        }
        sine_osc_process_params(voice, op);
    }
}

inline int32_t voice_process_audio(uint8_t voice)
{
    for (int op = 0; op < SYNTH_OPERATORS_PER_VOICE; op++)
    {
        sine_osc_process(voice, op);
        vca_process(voice, op);
        adsr_process_audio(voice, op);
    }
    bit_crusher_process_audio(voice);
    rate_reducer_process(voice);
    ring_mod_process_audio(voice);

    return audio_get_src_phase(voice, v[voice].params->src);
}

inline void voice_attach_params(uint8_t voice, uint8_t timbre)
{
    v[voice].params = &params[timbre];
}

void voice_init(uint8_t voice, uint8_t timbre, uint8_t alg)
{
    voice_attach_params(voice, timbre);

    for (int op = 0; op < SYNTH_OPERATORS_PER_VOICE; op++)
    {
        sine_osc_params_attach(voice, op, &params[timbre].osc_p[op]);
        adsr_params_attach(voice, op, 0, &params[timbre].adsr_p[op][0]);
        adsr_params_attach(voice, op, 1, &params[timbre].adsr_p[op][1]);
        vca_params_attach(voice, op, &params[timbre].vca_p[op]);
    }
    
    bit_crusher_params_attach(voice, &params[timbre].bc_p);
    rate_reducer_params_attach(voice, &params[timbre].rr_p);
    ring_mod_params_attach(voice, &params[timbre].rm_p);
}

enum cc_map 
{
    CC_ATTACK_OP_1,
    CC_DECAY_OP_1,
    CC_SUSTAIN_OP_1,
    CC_RELEASE_OP_1,
    CC_ADSR_EXP_OP_1,

    CC_ATTACK_P_OP_1,
    CC_DECAY_P_OP_1,
    CC_SUSTAIN_P_OP_1,
    CC_RELEASE_P_OP_1,

    CC_LEVEL_OP_1,

    CC_PITCH_TRACK_OP_1,
    CC_OCTAVE_OP_1,
    CC_TRANSPOSE_OP_1,
    CC_FINE_TUNE_OP_1,
    CC_MOD_DEPTH_OP_1,

    CC_ATTACK_OP_2,
    CC_DECAY_OP_2,
    CC_SUSTAIN_OP_2,
    CC_RELEASE_OP_2,
    CC_ADSR_EXP_OP_2,

    CC_ATTACK_P_OP_2,
    CC_DECAY_P_OP_2,
    CC_SUSTAIN_P_OP_2,
    CC_RELEASE_P_OP_2,

    CC_LEVEL_OP_2,

    CC_PITCH_TRACK_OP_2,
    CC_OCTAVE_OP_2,
    CC_TRANSPOSE_OP_2,
    CC_FINE_TUNE_OP_2,
    CC_MOD_DEPTH_OP_2,

    CC_RATE_REDUCE_AMOUNT,
    CC_BIT_CRUSH_AMOUNT,
    CC_RING_MOD_ENABLE,
};

inline void voice_handle_cc(uint8_t timbre, uint8_t controller, uint8_t value)
{
    voice_params_t * p = &params[timbre];

    switch (controller)
    {
        case CC_ATTACK_P_OP_1:
        case CC_DECAY_P_OP_1:
        case CC_SUSTAIN_P_OP_1:
        case CC_RELEASE_P_OP_1:
        break;

        case CC_ATTACK_P_OP_2:
        case CC_DECAY_P_OP_2:
        case CC_SUSTAIN_P_OP_2:
        case CC_RELEASE_P_OP_2:
        break;

        case CC_PITCH_TRACK_OP_1:
        sine_osc_params_set_pitch_tracking(&p->osc_p[0], value);
        break;

        case CC_PITCH_TRACK_OP_2:
        sine_osc_params_set_pitch_tracking(&p->osc_p[1], value);
        break;

        case CC_ATTACK_OP_1: 
        case CC_DECAY_OP_1:
        case CC_SUSTAIN_OP_1:
        case CC_RELEASE_OP_1:
        adsr_params_set(&p->adsr_p[0][ADSR_TYPE_AMP], controller - CC_ATTACK_OP_1, value);
        break;

        case CC_ADSR_EXP_OP_1:
        adsr_params_set_exp(&p->adsr_p[0][ADSR_TYPE_AMP], value);
        break;

        case CC_LEVEL_OP_1:
        vca_params_set_gain(&p->vca_p[0], value);
        break;

        case CC_OCTAVE_OP_1:
        sine_osc_params_set_octave_offset(&p->osc_p[0], value);
        break;

        case CC_TRANSPOSE_OP_1:
        sine_osc_params_set_transpose(&p->osc_p[0], value);
        break;

        case CC_FINE_TUNE_OP_1:
        sine_osc_params_set_fine_offset(&p->osc_p[0], value);
        break;

        case CC_MOD_DEPTH_OP_1:
        sine_osc_params_set_mod_amount(&p->osc_p[0], value);
        break;

        case CC_ATTACK_OP_2:
        case CC_DECAY_OP_2:
        case CC_SUSTAIN_OP_2:
        case CC_RELEASE_OP_2:
        adsr_params_set(&p->adsr_p[1][ADSR_TYPE_AMP], controller - CC_ATTACK_OP_2, value);
        break;

        case CC_ADSR_EXP_OP_2:
        adsr_params_set_exp(&p->adsr_p[1][ADSR_TYPE_AMP], value);
        break;

        case CC_LEVEL_OP_2:
        vca_params_set_gain(&p->vca_p[1], value);
        break;

        case CC_OCTAVE_OP_2:
        sine_osc_params_set_octave_offset(&p->osc_p[1], value);
        break;

        case CC_TRANSPOSE_OP_2:
        sine_osc_params_set_transpose(&p->osc_p[1], value);
        break;

        case CC_FINE_TUNE_OP_2:
        sine_osc_params_set_fine_offset(&p->osc_p[1], value);
        break;

        case CC_MOD_DEPTH_OP_2:
        sine_osc_params_set_mod_amount(&p->osc_p[1], value);
        break;

        case CC_RATE_REDUCE_AMOUNT:
        rate_reducer_params_set_amount(&p->rr_p, value);
        break;

        case CC_BIT_CRUSH_AMOUNT:
        bit_crusher_params_set_amount(&p->bc_p, value);
        break;

        case CC_RING_MOD_ENABLE:
        ring_mod_params_set_enable(&p->rm_p, value);
        break;
    }
}