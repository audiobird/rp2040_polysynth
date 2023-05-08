#include <midi_input_driver.h>

struct midi_istream midi_input[NUM_MIDI_INPUTS];
uint8_t sysex_buffer[NUM_MIDI_INPUTS][SIZE_OF_SYSEX_BUFFER]; 

static size_t read_uart_0(uint8_t *data)
{
    int size = uart_is_readable(uart0) > 0;
    
    //it doesnt matter if this data is garbage.
    //it will be handeled based on the return value of "size"
    *data = (uint8_t)uart_get_hw(uart0)->dr;

    return size;
}

void midi_input_driver_init()
{
    //remember this shit needs to be copied if there are more midi inputs.
    uart_init(uart0, 31250);
	gpio_set_function(1, GPIO_FUNC_UART);
    midi_input[0].sysex_buffer.data = sysex_buffer[0];
    midi_input[0].sysex_buffer.size = SIZE_OF_SYSEX_BUFFER;
    midi_input[0].read_cb = read_uart_0;
}

void midi_input_driver_read()
{
    struct midi_message *message = midi_decode(&midi_input[0]);

    if (message != NULL)
    {
        switch(message->type)
        {
            case MIDI_TYPE_NOTE_ON:
            {
                if (message->data.note_on.velocity)
                {
                    midi_handle_note_on(message);
                    break;
                }
               
            }
            case MIDI_TYPE_NOTE_OFF:
            midi_handle_note_off(message);
            break;

            case MIDI_TYPE_CONTROL_CHANGE:
            midi_handle_cc(message);
            break;            
        }
        
    }
}


    