#ifndef __ADSR_H__
#define __ADSR_H__

#include <pico/stdio.h>
#include "trig_gate.h"

typedef enum adsr_state
{
    ADSR_ATTACK,
    ADSR_DECAY,
    ADSR_SUSTAIN,
    ADSR_RELEASE,
} adsr_state_t;

typedef struct adsr_params
{
    int8_t a;
    int8_t d;
    int8_t s;
    int8_t r;    
} adsr_params_t;

typedef struct adsr
{
    adsr_params_t * params;
    adsr_state_t state;
    int32_t out_val;
    gate_t * gate;
} adsr_t;

void adsr_params_attach(adsr_t * adsr, adsr_params_t * params);
void adsr_attach_gate(adsr_t * adsr, gate_t * gate);

void adsr_params_set_attack(adsr_params_t * x, uint8_t m_val);
void adsr_params_set_decay(adsr_params_t * x, uint8_t m_val);
void adsr_params_set_sustain(adsr_params_t * x, uint8_t m_val);
void adsr_params_set_release(adsr_params_t * x, uint8_t m_val);

void adsr_process(adsr_t * x);
bool adsr_is_open(adsr_t * x);
void adsr_reset_hard(adsr_t * x);
int32_t adsr_get_output(adsr_t * x);

#endif