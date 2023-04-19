#include "sine_osc.h"
#include "tables/sine_table.h"

int16_t sine_osc_get_sample(sine_osc_t * o, int32_t phase_mod_input)
{
    uint16_t phase = phasor_get_phase(&o->phasor);
    phase += phase_mod_input;
    return sine_table[phase];
}

void sine_osc_process(sine_osc_t * o)
{
    phasor_advance_phase(&o->phasor);
}

void sine_osc_set_pitch(sine_osc_t * o, int8_t octave, int16_t semitone, int16_t fine_tune)
{
    phasor_set_phase_step(&o->phasor, phasor_create_tuning_word(octave, semitone, fine_tune));
}