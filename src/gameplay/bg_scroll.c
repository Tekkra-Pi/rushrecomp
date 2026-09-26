/* Gameplay level BG scroll functions. */

#include "nds_types.h"

void BgScroll_Init(void) { g_bgscroll_count = 0; }

s32 BgScroll_Add(s32 x, s32 y, u32 param) {
    if (g_bgscroll_count >= 16) return -1;
    u32 i = g_bgscroll_count;
    g_bgscrolls[i].bg_id = param;
    g_bgscrolls[i].x = x;
    g_bgscrolls[i].y = y;
    g_bgscrolls[i].offset_x = 0;
    g_bgscrolls[i].offset_y = 0;
    g_bgscrolls[i].speed_x = 0x100;
    g_bgscrolls[i].speed_y = 0;
    g_bgscrolls[i].active = 1;
    g_bgscroll_count++;
    return i;
}

void BgScroll_Remove(u32 index) {
    if (index < g_bgscroll_count) g_bgscrolls[index].active = 0;
}

void BgScroll_Update(void) {
    u32 i;
    for (i = 0; i < g_bgscroll_count; i++) {
        if (!g_bgscrolls[i].active) continue;
        g_bgscrolls[i].offset_x += g_bgscrolls[i].speed_x;
        g_bgscrolls[i].offset_y += g_bgscrolls[i].speed_y;
    }
}

void BgScroll_Draw(void) {
}

void BgScroll_GetOffset(u32 index) {
    (void)index;
}

void BgScroll_SetSpeed(u32 index, u32 value) {
    if (index < g_bgscroll_count) {
        g_bgscrolls[index].speed_x = (s32)value;
    }
}

s32 BgScroll_IsActive(u32 index) {
    if (index >= g_bgscroll_count) return 0;
    return g_bgscrolls[index].active;
}

u32 BgScroll_GetCount(void) { return g_bgscroll_count; }

void BgScroll_Clear(void) { g_bgscroll_count = 0; }
void BgScroll_Reset(void) { g_bgscroll_count = 0; }
