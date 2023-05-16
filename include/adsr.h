#ifndef __ADSR_H__
#define __ADSR_H__

#include <pico/stdio.h>

typedef enum adsr_state
{
    ADSR_OFF,
    ADSR_ATTACK,
    ADSR_DECAY,
    ADSR_SUSTAIN,
    ADSR_RELEASE,
} adsr_state_t;

typedef enum adsr_midi_params
{
    ADSR_P_A,
    ADSR_P_D,
    ADSR_P_S,
    ADSR_P_R,
} adsr_midi_param_t;

typedef enum adsr_types
{
    ADSR_TYPE_AMP,
    ADSR_TYPE_PITCH,
    ADSR_TYPE_CNT,
}adsr_types_t;

typedef struct adsr_params
{
    uint16_t a;
    uint16_t d;
    uint16_t s;
    uint16_t r;    
    bool exp;
} adsr_params_t;

typedef struct adsr
{
    adsr_params_t * params;
    adsr_state_t state;
    uint16_t out_val;
    int32_t working_val;
} adsr_t;

void adsr_params_attach(uint8_t voice, uint8_t op, adsr_types_t type, adsr_params_t * params);

void adsr_params_set(adsr_params_t * x, adsr_midi_param_t stage, uint8_t m_val);

void adsr_params_set_exp(adsr_params_t * x, uint8_t m_val);

void adsr_process(uint8_t voice, uint8_t op, adsr_types_t type);

bool adsr_is_open(uint8_t voice, uint8_t op, adsr_types_t type);

bool adsr_is_off(uint8_t voice, uint8_t op, adsr_types_t type);

int32_t adsr_get_output(uint8_t voice, uint8_t op, adsr_types_t type);

void adsr_start(uint8_t voice, uint8_t op, adsr_types_t type);
void adsr_release(uint8_t voice, uint8_t op, adsr_types_t type);

#endif