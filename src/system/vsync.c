/* VSync and display timing functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_system_flags;
extern u32 g_vsync_count;

void VSync_Wait(void) {
    u16 target = REG_VCOUNT;
    while (REG_VCOUNT == target) {}
}

u32 VSync_GetCount(void) {
    return g_vsync_count;
}

void Main_Init(void) {
    g_system_flags = 0;
    g_vsync_count = 0;
    REG_DISPCNT = 0;
    REG_DISPSTAT = 0;
}
