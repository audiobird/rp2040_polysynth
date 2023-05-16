#ifndef __VOICE_H__
#define __VOICE_H__

#include <pico/stdio.h>
#include "sine_osc.h"
#include "vca.h"
#include "adsr.h"
#include "bit_crusher.h"
#include "rate_reducer.h"
#include "ring_mod.h"
#include "signal.h"

typedef struct voice_params
{
    sine_osc_params_t osc_p[SYNTH_OPERATORS_PER_VOICE];
    vca_params_t vca_p[SYNTH_OPERATORS_PER_VOICE];
    adsr_params_t adsr_p[SYNTH_OPERATORS_PER_VOICE][ADSR_TYPE_CNT];
    bit_crusher_params_t bc_p;
    rate_reducer_params_t rr_p;
    ring_mod_params_t rm_p;
    signal_src_t src;
} voice_params_t;

void voice_process_params(uint8_t voice);
void voice_process_audio(uint8_t voice);
void voice_attach_params(uint8_t voice, uint8_t timbre);
void voice_init(uint8_t voice, uint8_t timbre, uint8_t alg);
audio_output_t voice_get_all();

void voice_note_on(uint8_t voice, uint8_t timber, uint8_t note, uint8_t velocity);
void voice_note_off(uint8_t voice);

void voice_main(uint8_t voice);
void voice_handle_cc(uint8_t timbre, uint8_t controller, uint8_t value);

#endif