#include "adsr.h"
#include "math.h"
#include "tables/exp_table.h"
#include "common.h"
#include "a_io.h"

static const int32_t ADSR_TOP = ADSR_TOP_; 

static const signal_dst_t adsr_dst[SYNTH_OPERATORS_PER_VOICE] = {SRC_ADSR0, SRC_ADSR1};

typedef struct adsr
{
    adsr_params_t * params;
    adsr_state_t state;
    int32_t out_val;
    int32_t next_val;
    int32_t inc_val;
    int32_t working_val;
} adsr_t;

static adsr_t adsr[SYNTH_NUM_VOICES][SYNTH_OPERATORS_PER_VOICE][ADSR_TYPE_CNT];

static inline uint16_t adsr_midi_to_rate(uint8_t mval)
{
    return exp_table[mval << 9] + 1;
}

static inline void adsr_params_set_attack(adsr_params_t * x, uint8_t m_val)
{
    x->a = adsr_midi_to_rate(m_val);
}

static inline void adsr_params_set_decay(adsr_params_t * x, uint8_t m_val)
{
    x->d = adsr_midi_to_rate(m_val);
}

static inline void adsr_params_set_sustain(adsr_params_t * x, uint8_t m_val)
{
    uint16_t val = 127 - m_val;
    val <<= 9;
    x->s = 65536 - exp_table[val];
}

static inline void adsr_params_set_release(adsr_params_t * x, uint8_t m_val)
{
    x->r = adsr_midi_to_rate(m_val);
}

inline void adsr_params_set(adsr_params_t * x, adsr_midi_param_t stage, uint8_t m_val)
{
    switch (stage)
    {
        case ADSR_P_A: adsr_params_set_attack(x, m_val);    break;
        case ADSR_P_D: adsr_params_set_decay(x, m_val);     break;
        case ADSR_P_S: adsr_params_set_sustain(x, m_val);   break;
        case ADSR_P_R: adsr_params_set_release(x, m_val);   break;
    }
}

inline void adsr_params_set_exp(adsr_params_t * x, uint8_t m_val)
{
    x->exp = midi_to_switch(m_val);
}

inline void adsr_process_envelope(uint8_t voice, uint8_t op, adsr_types_t type)
{
    adsr_t * x = &adsr[voice][op][type];

    switch (x->state)
    {
        case ADSR_ATTACK:
        {
            x->working_val += x->params->a;

            if (x->working_val > ADSR_TOP)
            {
                x->working_val = ADSR_TOP;
                x->state = ADSR_DECAY;
            }
            
            break;
        }
        case ADSR_DECAY:
        {
            x->working_val -= x->params->d;

            if (x->working_val < x->params->s)
            {
                x->working_val = x->params->s;
                x->state = ADSR_SUSTAIN;
            }
            
            break;
        }
        case ADSR_SUSTAIN: 
            x->working_val = x->params->s; 
            break;

        case ADSR_RELEASE:
        {
            x->working_val -= x->params->r;

            if (x->working_val < 0)
            {
                x->working_val = 0;
                x->state = ADSR_OFF;
            }

            break;
        }
        case ADSR_QUICK_RELEASE:
        x->working_val = 0;
        x->state = ADSR_ATTACK;
        break;

        case ADSR_OFF:
        break;
    }

    x->out_val = x->next_val;

    if (x->params->exp)
    x->next_val = (exp_table[x->working_val]);
    else
    x->next_val = (x->working_val);

    x->inc_val = (x->next_val - x->out_val) >> ADSR_SHIFT_VAL;  

    
}

inline void adsr_process_audio(uint8_t voice, uint8_t op)
{
    audio_output_t x = audio_get_src_phase(voice, adsr[voice][op][ADSR_TYPE_AMP].params->src);
    x = multiply_and_scale(x, adsr[voice][op][ADSR_TYPE_AMP].out_val, 16);
    adsr[voice][op][ADSR_TYPE_AMP].out_val += adsr[voice][op][ADSR_TYPE_AMP].inc_val;
    audio_set_dst_phase(voice, adsr_dst[op], x);
}

inline bool adsr_is_open(uint8_t voice, uint8_t op, adsr_types_t type)
{
    return adsr[voice][op][type].working_val > 0;
}

inline bool adsr_is_off(uint8_t voice, uint8_t op, adsr_types_t type)
{
    return !adsr_is_open(voice, op, type);
}

inline int32_t adsr_get_output(uint8_t voice, uint8_t op, adsr_types_t type)
{
    return adsr[voice][op][type].out_val;
}

inline void adsr_params_attach(uint8_t voice, uint8_t op, adsr_types_t type, adsr_params_t * params)
{
    adsr[voice][op][type].params = params;
}

inline void adsr_trig_voice(uint8_t voice)
{
    for(int op = 0; op < SYNTH_OPERATORS_PER_VOICE; op++)
    {
        for (int type = 0; type < ADSR_TYPE_CNT; type++)
        {
            adsr_t * x = &adsr[voice][op][type];
            x->state = ADSR_QUICK_RELEASE;
        }
    }
}

inline void adsr_release_voice(uint8_t voice)
{
    for(int op = 0; op < SYNTH_OPERATORS_PER_VOICE; op++)
    {
        for (int type = 0; type < ADSR_TYPE_CNT; type++)
        {
            adsr[voice][op][type].state = ADSR_RELEASE;
        }
    }
}