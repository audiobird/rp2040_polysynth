#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <midi_input_driver.h>

#include "mcp4921.h"

#include "voice.h"

/*  Notes:
Core 0 = Audio synthesis
Core 1 = Midi, CV, Gates, LFO

4 Voice
16 Part multi-timberal 

3 CV Inputs
? Gate inputs   
*/

volatile int xrun = 0;
volatile bool wait = 1;


voice_t voice[SYNTH_NUM_VOICES];
voice_params_t params[SYNTH_NUM_VOICES];

int32_t core_0_process_audio_rate()
{
    int32_t sample = 0;

    for (int x = 0; x < SYNTH_NUM_VOICES; x++)
    {
        voice_process_audio(&voice[x]);
        sample += *voice_get_output(&voice[x]);
    }
    
    return sample >> SYNTH_NUM_VOICES;
}

void core_0_process_audio_params()
{
    for(int x = 0; x < SYNTH_NUM_VOICES; x++)
    voice_process_params(&voice[x]);
}

void core_0()
{
    mcp4921_init();

    int32_t audio_buffer[SAMPLE_BUFFER_SIZE];

    for (int x = 0; x < SYNTH_NUM_VOICES; x++)
    voice_init(&voice[x], &params[x], 0);

    int iterations = 32;

    while(wait)
    ;

    while(iterations--)
    {

        core_0_process_audio_params();

        for (int x = 0; x < SAMPLE_BUFFER_SIZE; x++)
        {
            audio_buffer[x] = core_0_process_audio_rate();
        }

        xrun = !mcp4921_is_busy();

        mcp4921_send_buffer(audio_buffer);
    }

    while(1);
}

void core_1()
{
    midi_input_driver_init();
    gate_t flipper = 0;
    int8_t midi_note = 60;

    for (int x = 0; x < SYNTH_NUM_VOICES; x++)
    {   
        operator_params_t * op = params[x].op_params;

        op->amp_adsr_params.attack = 0;
        op->amp_adsr_params.decay = 0;
        op->amp_adsr_params.sustain = 0;
        op->amp_adsr_params.release = 0;

        op->vca_params.gain = 127;
    }
    
    params[1].op_params[0].sine_osc_params.transpose = 4;
    params[2].op_params[0].sine_osc_params.transpose = 7;
    params[3].op_params[0].sine_osc_params.transpose = 12;

    for(int x = 0; x < SYNTH_NUM_VOICES; x++)
    {
        voice_set_midi_note(&voice[x], 60);
        voice_set_gate(&voice[x], 1);
    }

    wait = 0;

    while(1)
    {   
        midi_input_driver_read();
        sleep_ms(1000);
        flipper ^= 1;
        
        
        
        if (flipper)
        midi_note++;

        if (midi_note == 72)
        midi_note = 60;

    }
}

int main()
{
    /* do general setup here, leds and whatnot */
    multicore_launch_core1(core_1);
    core_0();
    return 0;
}
