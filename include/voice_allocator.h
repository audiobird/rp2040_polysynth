#ifndef __VOICE_ALLOCATOR_H__
#define __VOICE_ALLOCATOR_H__

#include "pico/stdio.h"

//nothing here accounts for notes that may have a long release time....
//maybe something should be done to fix that?

typedef enum 
{
    VALLOC_OLDEST,
    VALLOC_NEWEST,
    VALLOC_BLOCK,
} voice_allocation_method_t;

typedef struct 
{
    voice_allocation_method_t method;
} voice_allocation_params_t;

typedef struct 
{
    uint8_t age[4];
    uint8_t note[4];
    uint8_t channel[4];
    uint8_t active_voices;
    voice_allocation_params_t * params;
} voice_allocation_t;

void voice_allocator_notify_note_on(uint8_t voice, uint8_t timber, uint8_t note, uint8_t velocity);
void voice_allocator_notify_note_off(uint8_t voice, uint8_t timber, uint8_t note);

bool voice_allocator_note_on(uint8_t channel, uint8_t note, uint8_t * best_voice);
bool voice_allocator_note_off(uint8_t channel, uint8_t note, uint8_t * voice);

#endif