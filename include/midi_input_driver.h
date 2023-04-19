#ifndef __MIDI_INPUT_DRIVER_H__
#define __MIDI_INPUT_DRIVER_H__

#include <stdio.h>
#include "pico/stdlib.h"

#define NUM_MIDI_INPUTS 1
#define SIZE_OF_SYSEX_BUFFER 32

void midi_input_driver_init();
void midi_input_driver_read();

#endif