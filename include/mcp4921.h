#ifndef __MCP4921_H__
#define __MCP4921_H__

#include "pico/stdlib.h"

#define MCP4921_DMA_CHAN 0

#define DAC_CONF (1u << 14 | 1u << 13 | 1u << 12)

void mcp4921_init();
void mcp4921_send_buffer(int32_t *);
bool mcp4921_is_busy();
int32_t mcp4921_get_timestamp();


#endif