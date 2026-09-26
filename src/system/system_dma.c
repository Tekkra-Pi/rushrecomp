/* System DMA functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern DmaChannelEntry g_dma_channels[];

#define MAX_DMA_CHANNELS 4

void DMA_Init(void) {
    u32 i;
    for (i = 0; i < MAX_DMA_CHANNELS; i++) {
        g_dma_channels[i].active = 0;
        g_dma_channels[i].callback = NULL;
    }
    REG_DMA0CNT = 0;
    REG_DMA1CNT = 0;
    REG_DMA2CNT = 0;
    REG_DMA3CNT = 0;
}

s32 DMA_Transfer(void) {
    /* Base DMA transfer - used internally */
    return 0;
}

void DMA_Stop(void) {
    REG_DMA0CNT = 0;
    REG_DMA1CNT = 0;
    REG_DMA2CNT = 0;
    REG_DMA3CNT = 0;
}

s32 DMA_IsActive(u32 index) {
    if (index >= MAX_DMA_CHANNELS) return 0;
    return g_dma_channels[index].active ? 1 : 0;
}

void DMA_Handler(void) {
    u32 i;
    for (i = 0; i < MAX_DMA_CHANNELS; i++) {
        if (g_dma_channels[i].active && g_dma_channels[i].callback) {
            g_dma_channels[i].callback();
        }
    }
}
