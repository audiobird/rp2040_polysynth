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
    ring_mod_params_t rm_params;
    signal_src_t src;
} voice_params_t;

typedef struct voice
{
    operator_t operator[SYNTH_OPERATORS_PER_VOICE];
    rate_reducer_t rate_reducer;
    bit_crusher_t bit_crusher;
    ring_mod_t ring_mod;

    voice_params_t * params;
    uint8_t timbre;
    int8_t midi_note;
    uint8_t velocity;
    int16_t pitch_bend;
    bool gate;
    bool prev_gate;
} voice_t;

void voice_process_params(uint8_t voice);
void voice_process_audio(uint8_t voice);
void voice_attach_params(uint8_t voice, uint8_t timbre);
void voice_init(uint8_t voice, uint8_t timbre, uint8_t alg);
audio_output_t voice_get_all();

void voice_note_on(uint8_t voice, uint8_t timber, uint8_t note, uint8_t velocity);
void voice_note_off(uint8_t voice);

void voice_main(uint8_t voice);

#endif