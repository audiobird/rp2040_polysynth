# RP2040 Poly Synth

- FM Based
- 16 Part multitimberal (Each midi channel can have a completely different patch and switch between them seamlessly)
- 4 Voices
    - 2 Sine Oscillators
    - 2 Amp ADSRs
    - 2 Pitch ADSRs
    - 2 VCAs
    - 1 Ring Mod / Sum Bus
    - 1 Bit Crusher
    - 1 Sample Rate Reducer
    - Route any audio src to any *module*
- 24khz sample rate

Currently using MCP4921 Dac on default PICO SPI Pins

Midi input on default PICO uart pins

### Road Map:a
- LFOs
- Patch storage
- Flexible patch randomization
