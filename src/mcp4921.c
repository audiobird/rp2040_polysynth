#include "mcp4921.h"
#include "hardware/spi.h"
#include "hardware/dma.h"
#include "hardware/interp.h"
#include <string.h>

const uint mcp4921_dma_chan = MCP4921_DMA_CHAN;

int16_t buffer[SAMPLE_BUFFER_SIZE] __attribute__((aligned(2048)));

void mcp4921_init()
{
    //setup spi pins
    spi_init(spi_default, 12000000);
    spi_set_format(spi_default, 16, 0, 0, SPI_MSB_FIRST);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);

    //setup dma
    dma_channel_config c = dma_channel_get_default_config(mcp4921_dma_chan);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
    int timer = dma_claim_unused_timer(true);
    dma_timer_set_fraction(timer, 6, 31250);
    int treq = dma_get_timer_dreq(timer);
    channel_config_set_dreq(&c, treq);

    for (int x = 0; x < 16; x++)
    if ((1u << x) & SAMPLE_BUFFER_SIZE)
    channel_config_set_ring(&c, false, x + 1);
    
    dma_channel_configure(mcp4921_dma_chan, &c, &spi_get_hw(spi_default)->dr, buffer, SAMPLE_BUFFER_SIZE, false);

    //setup clamping interpolator (overflowing 16 bit sample -> saturated 12 bit sample)
    interp_config cfg = interp_default_config();
    interp_config_set_clamp(&cfg, true);
    interp_config_set_shift(&cfg, 4);
    interp_config_set_mask(&cfg, 0, 27);
    interp_config_set_signed(&cfg, true);
    interp_set_config(interp1, 0, &cfg);
    interp1->base[0] = -2048;
    interp1->base[1] = 2047;
}

static inline int16_t clamp(int32_t samp)
{
    interp1->accum[0] = samp;
    return (int16_t)interp1->peek[0];
}

inline bool mcp4921_is_busy()
{
    return dma_channel_is_busy(mcp4921_dma_chan);
}

inline void mcp4921_send_buffer(int32_t * samples)
{
    while(mcp4921_is_busy())
    ;

    for (int x = 0; x < SAMPLE_BUFFER_SIZE; x++)
    {
        buffer[x] = clamp(samples[x]) + 2048;
        buffer[x] |= DAC_CONF;
    }
    //dma_channel_set_read_addr(mcp4921_dma_chan, buffer, true);
    dma_channel_start(mcp4921_dma_chan);   
}

inline int32_t mcp4921_get_timestamp()
{
    return SAMPLE_BUFFER_SIZE - dma_hw->ch[mcp4921_dma_chan].transfer_count;
}

