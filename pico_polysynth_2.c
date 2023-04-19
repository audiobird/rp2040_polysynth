#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <midi_input_driver.h>

#include "mcp4921.h"

#include "sine_osc.h"
#include "adsr.h"

/*  Notes:
Core 0 = Audio synthesis
Core 1 = Midi, CV, Gates, LFO

4 Voice
16 Part multi-timberal 

3 CV Inputs
? Gate inputs   
*/

int32_t core_0_process_audio_rate()
{
    int32_t sample = 0;
    return sample;
}

void core_0_process_audio_params()
{

}

void core_0()
{
    mcp4921_init();

    int32_t audio_buffer[SAMPLE_BUFFER_SIZE];

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

    while(1)
    {   
        midi_input_driver_read();
    }
}

int main()
{
    /* do general setup here, leds and whatnot */
    multicore_launch_core1(core_1);
    core_0();
    return 0;
}
