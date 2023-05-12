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
    ADSR_OFF,
} adsr_state_t;

typedef enum adsr_midi_params
{
    ADSR_P_A,
    ADSR_P_D,
    ADSR_P_S,
    ADSR_P_R,
} adsr_midi_param_t;

typedef struct adsr_params
{
    uint16_t a;
    uint16_t d;
    uint16_t s;
    uint16_t r;    
} adsr_params_t;

typedef struct adsr
{
    adsr_params_t * params;
    adsr_state_t state;
    uint16_t out_val;
    int32_t working_val;
} adsr_t;

void adsr_params_attach(adsr_t * adsr, adsr_params_t * params);

void adsr_params_set(adsr_params_t * x, adsr_midi_param_t adsr, uint8_t m_val);

void adsr_process(adsr_t * x);

bool adsr_is_open(adsr_t * x);

bool adsr_is_off(adsr_t * x);

int32_t adsr_get_output(adsr_t * x);

void adsr_start(adsr_t * x);
void adsr_release(adsr_t * x);

#endif