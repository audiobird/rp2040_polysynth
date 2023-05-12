#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "midi_input_driver.h"
#include "voice_allocator.h"

#include "mcp4921.h"

#include "voice.h"

#define dac_init mcp4921_init

volatile bool wait = 1;

void midi_handle_note_on(struct midi_message * message)
{
    uint8_t voice = 0;

    if(!voice_allocator_note_on(message->channel, message->data.note_on.note, &voice))
    return;

    //yea. trigger da note
    voice_note_on(voice, message->channel, message->data.note_on.note, message->data.note_on.velocity);
}

void midi_handle_note_off(struct midi_message * message)
{
    uint8_t voice = 0;

    if (!voice_allocator_note_off(message->channel, message->data.note_off.note, &voice))
    return;

    //trigger the note off.
    voice_note_off(voice);
}

void midi_handle_cc(struct midi_message * message)
{
   
}

int32_t core_0_process_audio_rate()
{
    int32_t sample = 0;

    for (int x = 0; x < SYNTH_NUM_VOICES; x++)
    {
        voice_process_audio(x);
    }
    
    return voice_get_all() / SYNTH_NUM_VOICES;
}

void core_0_process_audio_params()
{
    for(int x = 0; x < SYNTH_NUM_VOICES; x++)
    {
        voice_main(x);
        voice_process_params(x);
    }
   
}

void core_0()
{
    dac_init();

    int32_t audio_buffer[SAMPLE_BUFFER_SIZE];

    while(wait)
    ;

    wait = 1;

    while(1)
    {

        core_0_process_audio_params();

        for (int x = 0; x < SAMPLE_BUFFER_SIZE; x++)
        {
            audio_buffer[x] = core_0_process_audio_rate();
        }

        mcp4921_send_buffer(audio_buffer);
    }

    while(1);
}

void core_1()
{
    midi_input_driver_init();

    voice_init(0, 0, 0);
    voice_init(1, 0, 0);
    voice_init(2, 0, 0);
    voice_init(3, 0, 0);

    wait = 0;

    while(!wait)
    ;

    while(1)
    {   
        midi_input_driver_read();
    }
}

int main()
{
    //set_sys_clock_khz(250, 1);
    /* do general setup here, leds and whatnot */
    multicore_launch_core1(core_1);
    core_0();
    return 0;
}
