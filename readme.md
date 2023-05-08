# RP2040 Poly Synth

- FM Based
- 16 Part multitimberal (Each midi channel can have a completely different patch and switch between them seamlessly)
- 4 Voices
    - 2 Sine Oscillators
        - 1 Amp ADSR
        - 1 VCA
    - 1 Ring Mod
    - 1 Bit Crusher
    - 1 Sample Rate Reducer
    - Flexible Audio Routing
- 48khz sample rate

Currently using MCP4921 Dac on default PICO SPI Pins

### Needs testing: 
- Midi input (the code is there and running but I haven't actually tried using it, currently setup on default PICO uart rx pin)

### Road Map:
- Figure out voice allocating
- Add eurorack cv inputs?
- Patch storage
- Flexible patch randomization
