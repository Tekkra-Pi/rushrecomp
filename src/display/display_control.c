/* Display control functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

void Display_SetBrightness(u32 index, u32 value) {
    vu16 *bright = (index == 0) ? &REG_MASTER_BRIGHT : &REG_MASTER_BRIGHT_SUB;
    *bright = (u16)((value & 0x1F) | 0x8000);
}

void Display_SetMasterBrightness(u32 index, u32 value) {
    Display_SetBrightness(index, value);
}

void Display_WaitVBlankCount(void) {
    u16 vcount;
    do {
        vcount = REG_VCOUNT;
    } while (vcount >= 160);
    do {
        vcount = REG_VCOUNT;
    } while (vcount < 160);
}

void Display_FadeIn(void) {
    u32 i;
    for (i = 0; i < 16; i++) {
        REG_MASTER_BRIGHT = (u16)(0x10 - i);
        REG_MASTER_BRIGHT_SUB = (u16)(0x10 - i);
        Display_WaitVBlankCount();
    }
    REG_MASTER_BRIGHT = 0;
    REG_MASTER_BRIGHT_SUB = 0;
}
