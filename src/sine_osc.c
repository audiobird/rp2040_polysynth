#include "sine_osc.h"
#include "tables/sine_table.h"
#include "common.h"


void sine_osc_process(sine_osc_t * o)
{
    phasor_advance_phase(&o->phasor);
    uint16_t phase = phasor_get_phase(&o->phasor);
    phase += multiply_and_scale(*o->params->a_in, o->params->mod_amount, 7);
    o->a_out = sine_table[phase];
}

void sine_osc_set_pitch(sine_osc_t * o, int8_t octave, int16_t semitone, int16_t fine_tune)
{
    phasor_set_phase_step(&o->phasor, phasor_create_tuning_word(octave, semitone, fine_tune));
}

void sine_osc_params_set_octave_offset(sine_osc_params_t * p, int8_t octave_offset)
{ 
    p->octave_offset = clamp(octave_offset, -5, 5);
}

void sine_osc_params_set_transpose(sine_osc_params_t * p, int8_t transpose)
{
    p->transpose = clamp(transpose, -11, 11);
}

void sine_osc_params_set_mod_amount(sine_osc_params_t * p, int8_t mod_amount)
{
    p->mod_amount = mod_amount;
}

void sine_osc_params_set_fine_offset(sine_osc_params_t * p, int8_t fine_offset)
{
    p->fine_offset = clamp(fine_offset, -64, 63);
}

void sine_osc_params_attach(sine_osc_t * o, sine_osc_params_t * params)
{
    o->params = params;
}

void sine_osc_params_attach_audio_input(sine_osc_params_t * params, audio_output_t * src)
{
    params->a_in = src;
}

audio_output_t * sine_osc_get_output(sine_osc_t * o)
{
    return &o->a_out;
}