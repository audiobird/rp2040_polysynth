#include "adsr.h"
#include "math.h"
#include "tables/exp_table.h"

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
    x->exp = m_val >= 64;
}

void adsr_process(uint8_t voice, uint8_t op, adsr_types_t type)
{
    adsr_t * x = &adsr[voice][op][type];

    int temp = x->working_val;
    int rate;

    switch (x->state)
    {
        case ADSR_ATTACK:
        {
            rate = x->params->a;
            temp += rate;

            if (temp > UINT16_MAX)
            {
                temp = UINT16_MAX;
                x->state = ADSR_DECAY;
            }
            
            break;
        }
        case ADSR_DECAY:
        {
            rate = x->params->d;
            temp -= rate;

            if (temp < x->params->s)
            {
                temp = x->params->s;
                x->state = ADSR_SUSTAIN;
            }
            
            break;
        }
        case ADSR_SUSTAIN: 
            temp = x->params->s; 
            break;

        case ADSR_RELEASE:
        {
            temp -= x->params->r;

            if (temp < 0)
            {
                temp = 0;
                x->state = ADSR_OFF;
            }

            break;
        }
        case ADSR_OFF:
        break;
    }

    x->working_val = temp;

    if (x->params->exp)
    x->out_val = exp_table[temp];
    else
    x->out_val = temp;
}

inline bool adsr_is_open(uint8_t voice, uint8_t op, adsr_types_t type)
{
    return adsr[voice][op][type].working_val > 0;
}

inline bool adsr_is_off(uint8_t voice, uint8_t op, adsr_types_t type)
{
    return !adsr_is_open(voice, op, type);
}

int32_t adsr_get_output(uint8_t voice, uint8_t op, adsr_types_t type)
{
    return adsr[voice][op][type].out_val;
}

void adsr_params_attach(uint8_t voice, uint8_t op, adsr_types_t type, adsr_params_t * params)
{
    adsr[voice][op][type].params = params;
}

void adsr_start(uint8_t voice, uint8_t op, adsr_types_t type)
{
    adsr_t * x = &adsr[voice][op][type];
    x->working_val = 0;
    x->out_val = 0;
    x->state = ADSR_ATTACK;
}

void adsr_release(uint8_t voice, uint8_t op, adsr_types_t type)
{
    adsr[voice][op][type].state = ADSR_RELEASE;
}