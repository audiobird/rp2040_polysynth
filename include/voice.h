#ifndef __VOICE_H__
#define __VOICE_H__

#include <pico/stdio.h>
#include "operator.h"
#include "bit_crusher.h"
#include "rate_reducer.h"
#include "ring_mod.h"
#include "signal.h"
#include "trig_gate.h"

typedef struct voice_params
{
    operator_params_t op_params[SYNTH_OPERATORS_PER_VOICE];
    bit_crusher_params_t bc_params;
    rate_reducer_params_t rr_params;
    ring_mod_params_t rb_params;
} voice_params_t;

typedef struct voice
{
    operator_t operator[SYNTH_OPERATORS_PER_VOICE];
    rate_reducer_t rate_reducer;
    bit_crusher_t bit_crusher;
    ring_mod_t ring_mod;
    audio_output_t a_out;
    voice_params_t * params;
    int8_t midi_note;
    gate_t gate;
} voice_t;

audio_output_t * voice_get_output(voice_t * voice);
void voice_process_params(voice_t * voice);
void voice_process_audio(voice_t * voice);
void voice_set_gate(voice_t * voice, gate_t state);
void voice_set_midi_note(voice_t * voice, int8_t midi_note);
void voice_attach_params(voice_t * voice, voice_params_t * params);
void voice_init(voice_t * voice, voice_params_t * params, uint8_t alg);

#endif