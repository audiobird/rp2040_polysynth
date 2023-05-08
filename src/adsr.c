#include "adsr.h"
#include "math.h"
#include "tables/log_table.h"

uint16_t adsr_midi_to_rate(uint8_t mval)
{
    assert(mval < 128);
    return log_table_get(127 - mval) + 1;
}

void adsr_params_set_attack(adsr_params_t * x, uint8_t m_val)
{
    x->a = adsr_midi_to_rate(m_val);
    
}

void adsr_params_set_decay(adsr_params_t * x, uint8_t m_val)
{
    x->d = adsr_midi_to_rate(m_val);
}

void adsr_params_set_sustain(adsr_params_t * x, uint8_t m_val)
{
    x->s = adsr_midi_to_rate(m_val);
}

void adsr_params_set_release(adsr_params_t * x, uint8_t m_val)
{
    x->r = adsr_midi_to_rate(m_val);
}

void adsr_process(adsr_t * x)
{
    int temp = x->out_val;

    if (!*x->gate)
    x->state = ADSR_RELEASE;

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
            temp = 0;

            if (*x->gate)
            x->state = ADSR_ATTACK;

            break;
        }
    }

    x->out_val = temp;
}

bool adsr_is_open(adsr_t * x)
{
    return x->out_val > 0;
}

void adsr_reset_hard(adsr_t * x)
{
    x->out_val = 0;
    x->state = ADSR_ATTACK;
}

int32_t adsr_get_output(adsr_t * x)
{
    return x->out_val;
}

void adsr_params_attach(adsr_t * adsr, adsr_params_t * params)
{
    adsr->params = params;
}

void adsr_attach_gate(adsr_t * adsr, gate_t * gate)
{
   //gate_attach_src(adsr->gate, gate);
    adsr->gate = gate;
}