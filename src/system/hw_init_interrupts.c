/* Early ARM9 hardware init - interrupts and timers. */
#include "nds_types.h"

void HwInit_Interrupts(void) {
    *(volatile u32 *)0x04000208 = 0; /* IME off */
    *(volatile u32 *)0x04000210 = 0; /* IE clear */
    *(volatile u32 *)0x04000218 = 0xFFFFFFFF; /* IF clear */
    *(volatile u32 *)0x04000214 = 0; /* posting ack */
}

void HwInit_Timers(void) {
    volatile u32 *tmr = (volatile u32 *)0x04000100;
    u32 i;
    for (i = 0; i < 4; i++) {
        tmr[i * 4] = 0;      /* TMxCNT_L */
        tmr[i * 4 + 1] = 0;  /* TMxCNT_H (disable) */
    }
}

void HwInit_DMA(void) {
    volatile u32 *dma = (volatile u32 *)0x040000B0;
    u32 i;
    for (i = 0; i < 4; i++) {
        dma[i * 4] = 0;     /* DMAxSAD */
        dma[i * 4 + 1] = 0; /* DMAxDAD */
        dma[i * 4 + 2] = 0; /* DMAxCNT_L */
        dma[i * 4 + 3] = 0; /* DMAxCNT_H (disable) */
    }
}
