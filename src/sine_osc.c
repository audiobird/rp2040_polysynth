#include "sine_osc.h"
#include "tables/sine_table.h"
#include "common.h"

const signal_dst_t dst[SYNTH_OPERATORS_PER_VOICE] = {SRC_OSC0, SRC_OSC1};

void sine_osc_process(sine_osc_t * o, uint8_t voice, uint8_t op)
{
    phasor_advance_phase(&o->phasor);
    uint16_t phase = phasor_get_phase(&o->phasor);
    audio_input_t mod = audio_get_src_phase(voice, o->params->src);
    phase += multiply_and_scale(mod, o->params->mod_amount, 7);
    audio_set_dst_phase(voice, dst[op], sine_table[phase]);
}

void sine_osc_process_params(sine_osc_t * o, int8_t midi_note)
{
    phasor_tuning_word_t tune = phasor_create_tuning_word(  o->params->octave_offset,
                                                            o->params->transpose + midi_note, 
                                                            o->params->fine_offset);
    phasor_set_phase_step(&o->phasor, tune);

    if(is_trig(&o->trig))
    phasor_reset_phase(&o->phasor);
}

void sine_osc_params_set_octave_offset(sine_osc_params_t * p, int8_t m_val)
{ 
    m_val -= 64;
    p->octave_offset = clamp(m_val, -5, 5);
}

void sine_osc_params_set_transpose(sine_osc_params_t * p, int8_t m_val)
{
    m_val -= 64;
    p->transpose = clamp(m_val, -12, 12);
}

void sine_osc_params_set_fine_offset(sine_osc_params_t * p, int8_t m_val)
{   
    m_val -= 64;
    p->fine_offset = m_val;
}

void sine_osc_params_set_mod_amount(sine_osc_params_t * p, int8_t m_val)
{
    p->mod_amount = m_val;
}

void sine_osc_params_attach(sine_osc_t * o, sine_osc_params_t * params)
{
    o->params = params;
}

void sine_osc_reset_phase(sine_osc_t * o)
{
    o->phasor.accumulator = 0;
}