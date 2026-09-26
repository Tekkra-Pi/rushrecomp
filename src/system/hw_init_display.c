/* Early ARM9 hardware init - display. */
#include "nds_types.h"

void HwInit_Display(void) {
    /* Power down display components */
    *(volatile u32 *)0x04000304 = 0x00000000; /* POWCNT1 */
    /* Disable all display */
    *(volatile u16 *)0x04000000 = 0x0000; /* DISPCNT */
    *(volatile u16 *)0x04001000 = 0x0000; /* DISPCNT sub */
    /* Clear backgrounds */
    u32 i;
    for (i = 0; i < 4; i++) {
        *(volatile u16 *)(0x04000008 + i * 2) = 0;
        *(volatile u16 *)(0x04001008 + i * 2) = 0;
    }
    /* Clear OAM */
    for (i = 0; i < 0x400; i += 4) {
        *(volatile u32 *)(0x07000000 + i) = 0x200;
        *(volatile u32 *)(0x07000000 + i + 0x400) = 0x200;
    }
}

void HwInit_PowerOn(void) {
    /* Power on LCD and 2D engine */
    *(volatile u32 *)0x04000304 = 0x8203;
}
