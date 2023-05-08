#ifndef __MIDI_CC_H__
#define __MIDI_CC_H__

#include "pico/stdio.h"
#include "midi_input_driver.h"

typedef enum 
{
    CC_BANK_SELECT = 0,
    CC_MODULATION_WHEEL = 1,
    
    CC_DATA_ENTRY_MSB = 6,



    CC_DATA_ENTRY_LSB = 38,



    CC_DATA_INC = 96,
    CC_DATA_DEC = 97,
    CC_NRPN_LSB = 98,
    CC_NRPN_MSB = 99,
    CC_RPN_LSB = 100,
    CC_RPN_MSB = 101,



    CC_CHANNEL_MODE_0 = 120,
    CC_CHANNEL_MODE_1 = 121,
    CC_CHANNEL_MODE_2 = 122,
    CC_CHANNEL_MODE_3 = 123,
    CC_CHANNEL_MODE_4 = 124,
    CC_CHANNEL_MODE_5 = 125,
    CC_CHANNEL_MODE_6 = 126,
    CC_CHANNEL_MODE_7 = 127,
} midi_cc_map_t;

#endif