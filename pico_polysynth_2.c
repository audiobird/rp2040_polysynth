#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <midi_input_driver.h>

#include "mcp4921.h"

#include "operator.h"

/*  Notes:
Core 0 = Audio synthesis
Core 1 = Midi, CV, Gates, LFO

4 Voice
16 Part multi-timberal 

3 CV Inputs
? Gate inputs   
*/

audio_input_t no_gain = 32767;

operator_t op;
operator_params_t params[2] = {
    {
        .amp_adsr_params = {
        .attack = 96,
        .decay = 96,
        .sustain = 64,
        .release = 64,
    },
    .sine_osc_params = {
        .transpose = 69,
        .mod_amount = 0,
    },
    .vca_params = {
        .cv_in = &no_gain,
        .gain = 127,
    }
    },
    {
        .amp_adsr_params = {
        .attack = 96,
        .decay = 96,
        .sustain = 64,
        .release = 64,
    },
    .sine_osc_params = {
        .transpose = 48,
        .mod_amount = 32,
    },
    .vca_params = {
        .cv_in = &no_gain,
        .gain = 127,
    }
    }
    
};

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

    operator_init(&op, &params[0]);

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


    int patch = 0;
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
