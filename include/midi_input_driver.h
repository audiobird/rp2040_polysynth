#ifndef __MIDI_INPUT_DRIVER_H__
#define __MIDI_INPUT_DRIVER_H__

#include <stdio.h>
#include "pico/stdlib.h"
#include "nanomidi/decoder.h"

#define NUM_MIDI_INPUTS 1
#define SIZE_OF_SYSEX_BUFFER 32

void midi_input_driver_init();
void midi_input_driver_read();

void midi_handle_note_on(struct midi_message * message);
void midi_handle_note_off(struct midi_message * message);
void midi_handle_cc(struct midi_message * message);
void midi_handle_pitch_bend(struct midi_message * message);

#endif