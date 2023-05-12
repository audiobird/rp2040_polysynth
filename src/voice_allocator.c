#include "voice_allocator.h"

voice_allocation_t valloc = 
{
    .note[0 ... SYNTH_NUM_VOICES - 1] = 0xff,
};

bool voice_allocator_note_on(uint8_t channel, uint8_t note, uint8_t * best_voice)
{
    if (valloc.params->method == VALLOC_BLOCK && valloc.active_voices == SYNTH_NUM_VOICES)
    return 0;

    if (valloc.active_voices != SYNTH_NUM_VOICES)
    {
        for (*best_voice = 0; *best_voice < SYNTH_NUM_VOICES; *best_voice++)
        {
            if (valloc.note[*best_voice] == 0xff)
            {
                valloc.active_voices += 1;
                break;
            }
            else
            {
                while(1)
                ; //this should not be possible!
            }
        }
    }
    else
    {
        //all voices used.
        if (valloc.params->method == VALLOC_NEWEST)
        {
            //replace youngest note
            for (int x = 1; x < SYNTH_NUM_VOICES; x++)
            {
                if (valloc.age[x] < valloc.age[*best_voice])
                {
                    *best_voice = x;
                }
            }
        }
        else
        {
            //replace oldest note
            for (int x = 1; x < SYNTH_NUM_VOICES; x++)
            {
                if (valloc.age[x] > valloc.age[*best_voice])
                {
                    *best_voice = x;
                }
            }
        }
    }

    for (int x = 0; x < SYNTH_NUM_VOICES; x++)
    {
        valloc.age[x] += 1;
    }

    valloc.note[*best_voice] = note;
    valloc.channel[*best_voice] = channel;
    valloc.age[*best_voice] = 0;

    return 1;
}

bool voice_allocator_note_off(uint8_t channel, uint8_t note, uint8_t * voice)
{
    for (int x = 0; x < SYNTH_NUM_VOICES; x++)
    {
        if (valloc.note[x] == note && valloc.channel[x] == channel)
        {
            *voice = x;
            valloc.note[x] = 0xff;
            valloc.active_voices -= 1;
            return 1;
        }
    }
    return 0;
}

void voice_allocator_params_attach(voice_allocation_params_t * params)
{
    valloc.params = params;
}