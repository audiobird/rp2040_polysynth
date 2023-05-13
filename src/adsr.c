#include "adsr.h"
#include "math.h"
#include "tables/log_table.h"
#include "tables/exp_table.h"

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

inline void adsr_params_set(adsr_params_t * x, adsr_midi_param_t adsr, uint8_t m_val)
{
    switch (adsr)
    {
        case ADSR_P_A: adsr_params_set_attack(x, m_val);    break;
        case ADSR_P_D: adsr_params_set_decay(x, m_val);     break;
        case ADSR_P_S: adsr_params_set_sustain(x, m_val);   break;
        case ADSR_P_R: adsr_params_set_release(x, m_val);   break;
    }
}

void adsr_process(adsr_t * x)
{
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
        {
            temp = 0;
            break;
        }
    }

    x->working_val = temp;
    x->out_val = exp_table[temp];
}

inline bool adsr_is_open(adsr_t * x)
{
    return x->working_val > 0;
}

inline bool adsr_is_off(adsr_t * x)
{
    return !adsr_is_open(x);
}

int32_t adsr_get_output(adsr_t * x)
{
    return x->out_val;
}

void adsr_params_attach(adsr_t * adsr, adsr_params_t * params)
{
    adsr->params = params;
}

void adsr_start(adsr_t * x)
{
    x->working_val = 0;
    x->out_val = 0;
    x->state = ADSR_ATTACK;
}

void adsr_release(adsr_t * x)
{
    x->state = ADSR_RELEASE;
}