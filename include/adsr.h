#ifndef __ADSR_H__
#define __ADSR_H__

#include <pico/stdio.h>

typedef enum adsr_state
{
    ADSR_ATTACK,
    ADSR_DECAY,
    ADSR_SUSTAIN,
    ADSR_RELEASE,
} adsr_state_t;

typedef struct adsr_params
{
    int8_t attack;
    int8_t decay;
    int8_t sustain;
    int8_t release;
} adsr_params_t;

typedef struct adsr
{
    int attack;
    int decay;
    int sustain;
    int release;
    adsr_state_t state;
    int out_val;
    int trig;
} adsr_t;

void adsr_set_attack(adsr_t * x, uint8_t m_val);
void adsr_set_decay(adsr_t * x, uint8_t m_val);
void adsr_set_sustain(adsr_t * x, uint8_t m_val);
void adsr_set_release(adsr_t * x, uint8_t m_val);

void adsr_process(adsr_t * x);
void adsr_trig(adsr_t * x, bool dir);
bool adsr_is_open(adsr_t * x);
uint16_t adsr_get_output(adsr_t * x);

#endif