# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/cantrell/pico/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(pico_polysynth_2 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(pico_polysynth_2 
pico_polysynth_2.c 
src/nanomidi_decoder.c
src/midi_input_driver.c
src/mcp4921.c
src/adsr.c
src/phasor.c 
src/sine_osc.c
src/vca.c
src/operator.c 
src/voice.c
src/bit_crusher.c
src/rate_reducer.c
src/ring_mod.c
src/a_io.c
src/midi_cc.c
src/voice_allocator.c
include/voice_allocator.h
include/midi_cc.h
include/a_io.h
include/trig_gate.h
include/ring_mod.h
include/rate_reducer.h
include/bit_crusher.h
include/voice.h
include/operator.h
include/signal.h
include/vca.h
include/common.h
include/sine_osc.h
include/phasor.h
include/tables/exp_table.c
include/tables/exp_table.h
include/tables/phase_step_table.c
include/tables/phase_step_table.h
include/tables/sine_table.c
include/tables/sine_table.h
include/adsr.h
include/mcp4921.h
include/midi_input_driver.h
include/nanomidi/decoder.h
include/nanomidi/messages.h
include/nanomidi/nanomidi_internal.h)

pico_set_program_name(pico_polysynth_2 "pico_polysynth_2")
pico_set_program_version(pico_polysynth_2 "0.1")

pico_enable_stdio_uart(pico_polysynth_2 0)
pico_enable_stdio_usb(pico_polysynth_2 0)

# Add the standard library to the build
target_link_libraries(pico_polysynth_2
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(pico_polysynth_2 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
  include/
  )

# Add any user requested libraries
target_link_libraries(pico_polysynth_2
pico_multicore
hardware_spi
hardware_dma
hardware_interp
        )

pico_add_extra_outputs(pico_polysynth_2)

set(WAVE_TABLE_BIT_WIDTH "16")
set(WAVE_TABLE_SAMPLE_DEPTH "16")
set(AUDIO_SAMPLE_RATE "24000")
set(SAMPLE_BUFFER_SIZE "32")
set(PHASE_STEP_TABLE_BIT_SIZE "14")
set(EXP_CURVE "8") # curve <= 16

add_compile_definitions(
WAVE_TABLE_BIT_WIDTH=${WAVE_TABLE_BIT_WIDTH}
WAVE_TABLE_SAMPLE_DEPTH=${WAVE_TABLE_SAMPLE_DEPTH}
AUDIO_SAMPLE_RATE=${AUDIO_SAMPLE_RATE}
SAMPLE_BUFFER_SIZE=${SAMPLE_BUFFER_SIZE}
PHASE_STEP_TABLE_BIT_SIZE=${PHASE_STEP_TABLE_BIT_SIZE}
SYNTH_NUM_VOICES=4
SYNTH_NUM_TIMBRES=16
SYNTH_OPERATORS_PER_VOICE=2
#DIVIDE_FAST_SHIFT
#DIVIDE_FAST_ADJUSTED
DIVIDE_ACCURATE
)

add_custom_target(
    table_generators
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/utils/bin/sin_table_gen ${CMAKE_CURRENT_SOURCE_DIR}/include/tables/ ${WAVE_TABLE_BIT_WIDTH} ${WAVE_TABLE_SAMPLE_DEPTH}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/utils/bin/phase_step_table_gen ${CMAKE_CURRENT_SOURCE_DIR}/include/tables/ ${PHASE_STEP_TABLE_BIT_SIZE} ${AUDIO_SAMPLE_RATE}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/utils/bin/exp_table_gen ${CMAKE_CURRENT_SOURCE_DIR}/include/tables/ ${EXP_CURVE}
)
add_dependencies(pico_polysynth_2 table_generators)

