#include "sine_osc.h"
#include "tables/phase_step_table.h"
#include "tables/sine_table.h"
#include "tables/exp_table.h"
#include "common.h"

const uint8_t shift_val = 32 - WAVE_TABLE_BIT_WIDTH;

#define SYNTH_NUM_PHASORS (SYNTH_NUM_VOICES * SYNTH_OPERATORS_PER_VOICE)
#define M_NOTE_TO_PHASE_TABLE(a) (uint16_t)(a << (PHASE_STEP_TABLE_BIT_SIZE - 7))
#define PHASE_STEP_TABLE_SIZE (1u << PHASE_STEP_TABLE_BIT_SIZE)


typedef struct phasor
{
    uint32_t accumulator;
    uint32_t phase_step;
} phasor_t;

typedef struct sin_osc
{
    phasor_t phasor;
    sine_osc_params_t * params;
} sine_osc_t;


static const signal_dst_t o_dst[SYNTH_OPERATORS_PER_VOICE] = {SRC_OSC0, SRC_OSC1};

static sine_osc_t o[SYNTH_NUM_VOICES][SYNTH_OPERATORS_PER_VOICE];

static uint16_t midi_note[SYNTH_NUM_VOICES];

static inline void phasor_advance_phase(uint8_t voice, uint8_t op)
{
    o[voice][op].phasor.accumulator += o[voice][op].phasor.phase_step;
}

static inline uint16_t phasor_get_phase(uint8_t voice, uint8_t op)
{
    return o[voice][op].phasor.accumulator >> shift_val;
}

static inline void phasor_reset_phase(uint8_t voice, uint8_t op)
{
    o[voice][op].phasor.accumulator = 0;
}

static inline void phasor_set_phase_step(uint8_t voice, uint8_t op, uint16_t t)
{
    o[voice][op].phasor.phase_step = phase_step_table[t];
}

static inline void sine_osc_voice_set_midi_note(uint8_t voice, int8_t mnote)
{
    midi_note[voice] = mnote << (PHASE_STEP_TABLE_BIT_SIZE - 7);
}

inline void sine_osc_trig_voice(uint8_t voice, int8_t mnote)
{
    sine_osc_voice_set_midi_note(voice, mnote);

    for (int x = 0; x < SYNTH_OPERATORS_PER_VOICE; x++)
    phasor_reset_phase(voice, x);
}

inline void sine_osc_process(uint8_t voice, uint8_t op)
{
    phasor_advance_phase(voice, op);
    uint16_t phase = phasor_get_phase(voice, op);
    audio_input_t mod = audio_get_src_phase(voice, o[voice][op].params->src);
    phase += multiply_and_scale(mod, o[voice][op].params->mod_amount, 16);
    audio_set_dst_phase(voice, o_dst[op], sine_table[phase]);
}

inline void sine_osc_process_params(uint8_t voice, uint8_t op, int16_t bend)
{
    sine_osc_t * x = &o[voice][op];

    int tune = x->params->octave_offset + x->params->transpose + x->params->fine_offset;

    tune += x->params->track_pitch ? midi_note[voice] : 64 << (PHASE_STEP_TABLE_BIT_SIZE - 7);

    tune += multiply_and_scale(x->params->pitch_bend_amount, bend, 6);

    tune = clamp(tune, 0, PHASE_STEP_TABLE_SIZE - 1);

    phasor_set_phase_step(voice, op, tune);
}

inline void sine_osc_params_set_octave_offset(sine_osc_params_t * p, int8_t m_val)
{ 
    int temp = m_val - 64;
    temp = clamp(temp, -5, 5);
    temp *= 12;
    p->octave_offset = temp << (PHASE_STEP_TABLE_BIT_SIZE - 7);
}

inline void sine_osc_params_set_transpose(sine_osc_params_t * p, int8_t m_val)
{
    int temp = m_val - 64;
    temp = clamp(temp, -12, 12);
    p->transpose = temp << (PHASE_STEP_TABLE_BIT_SIZE - 7);
}

inline void sine_osc_params_set_fine_offset(sine_osc_params_t * p, int8_t m_val)
{   
    m_val -= 64;
    p->fine_offset = m_val;
}

inline void sine_osc_params_set_mod_amount(sine_osc_params_t * p, int8_t m_val)
{
    p->mod_amount = exp_table[m_val << 9];
}

inline void sine_osc_params_set_pitch_tracking(sine_osc_params_t * p, int8_t m_val)
{
    p->track_pitch = midi_to_switch(m_val);
}

inline void sine_osc_params_set_pitch_bend_amount(sine_osc_params_t * p, int8_t m_val)
{
    p->pitch_bend_amount = clamp(m_val, 0, 24);
}   

inline void sine_osc_params_attach(uint8_t voice, uint8_t op, sine_osc_params_t * params)
{
    o[voice][op].params = params;
}

static inline void sine_osc_reset_phase(uint8_t voice, uint8_t op)
{
    o[voice][op].phasor.accumulator = 0;
}