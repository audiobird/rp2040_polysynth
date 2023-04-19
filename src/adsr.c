#include "adsr.h"
#include "math.h"
#include "tables/log_table.h"

uint16_t adsr_midi_to_rate(uint8_t mval)
{
    assert(mval < 128);
    return log_table_get(127 - mval) + 1;
}

void adsr_set_attack(adsr_t * x, uint8_t m_val)
{
    x->attack = adsr_midi_to_rate(m_val);
}

void adsr_set_decay(adsr_t * x, uint8_t m_val)
{
    x->decay = adsr_midi_to_rate(m_val);
}

void adsr_set_sustain(adsr_t * x, uint8_t m_val)
{
    x->sustain = adsr_midi_to_rate(m_val);
}

void adsr_set_release(adsr_t * x, uint8_t m_val)
{
    x->release = adsr_midi_to_rate(m_val);
}

void adsr_process(adsr_t * x)
{
    int temp = x->out_val;

    if (!x->trig)
    x->state = ADSR_RELEASE;

    switch (x->state)
    {
        case ADSR_ATTACK:
        {
            temp += x->attack;

            if (temp > UINT16_MAX)
            {
                temp = UINT16_MAX;
                x->state = ADSR_DECAY;
            }
            
            break;
        }
        case ADSR_DECAY:
        {
            temp -= x->decay;

            if (temp < x->sustain)
            {
                temp = x->sustain;
                x->state = ADSR_SUSTAIN;
            }
            
            break;
        }
        case ADSR_SUSTAIN: 
            temp = x->sustain; 
            break;

        case ADSR_RELEASE:
        {
            temp -= x->release;

            if (temp < 0)
            temp = 0;

            if (x->trig)
            x->state = ADSR_ATTACK;

            break;
        }
    }

    x->out_val = temp;
}

void adsr_trig(adsr_t * x, bool dir)
{
    x->trig = dir;
}

bool adsr_is_open(adsr_t * x)
{
    return x->out_val > 0;
}

uint16_t adsr_get_output(adsr_t * x)
{
    return x->out_val;
}
