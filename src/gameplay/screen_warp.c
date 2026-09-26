/* Gameplay level screen warp functions. */
#include "nds_types.h"

void ScreenWarp_Init(void) { }

s32 ScreenWarp_Add(s32 x, s32 y, u32 dest_x, u32 dest_y) {
    u32 i = 0;
    g_screenwarps[i].x = x;
    g_screenwarps[i].y = y;
    g_screenwarps[i].dest_x = dest_x;
    g_screenwarps[i].dest_y = dest_y;
    g_screenwarps[i].active = 1;
    return i;
}

void ScreenWarp_Update(void) { }

void ScreenWarp_Remove(u32 index) {
    if (index < 16) g_screenwarps[index].active = 0;
}

u32 ScreenWarp_GetCount(void) { return 16; }

void ScreenWarp_Clear(void) {
    u32 i;
    for (i = 0; i < 16; i++) g_screenwarps[i].active = 0;
}

void ScreenWarp_Reset(void) { ScreenWarp_Clear(); }
