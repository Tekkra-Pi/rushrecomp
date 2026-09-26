/* DMA and timer functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern DmaChannelEntry g_dma_channels[];

void DMA_ChainTransfer(u32 channel, void *src, void *dst, u32 len, u32 control) {
    vu32 *regs;
    if (channel > 3) return;
    regs = (vu32*)(0x040000B0 + channel * 12);
    regs[0] = (u32)src;
    regs[1] = (u32)dst;
    regs[2] = len | control | DMA_ENABLE | DMA_START_NOW;
}

void Timer_Set(u32 index, u32 value) {
    vu16 *timer_d;
    vu32 *timer_cnt;
    if (index > 3) return;
    timer_d = (vu16*)(0x04000100 + index * 4);
    timer_cnt = (vu32*)(0x04000102 + index * 4);
    *timer_cnt = 0;
    *timer_d = (u16)value;
    *timer_cnt = TIMER_ENABLE;
}

u16 Timer_Get(void) {
    /* Returns timer 0 count */
    return REG_TM0D;
}
