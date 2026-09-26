/* Gameplay level display effect functions. */

#include "nds_types.h"

#define DISPEFFECT_MAX 16

static u32 s_dispeffect_count;

s32 DisplayEffect_Add(s32 x, s32 y, u32 param) {
    if (s_dispeffect_count >= DISPEFFECT_MAX) return -1;
    u32 i = s_dispeffect_count;
    g_explosionfxs[i].x = x;
    g_explosionfxs[i].y = y;
    g_explosionfxs[i].size = param;
    g_explosionfxs[i].frame = 0;
    g_explosionfxs[i].timer = 0;
    g_explosionfxs[i].active = 1;
    s_dispeffect_count++;
    return i;
}

void DisplayEffect_Remove(u32 index) {
    if (index < DISPEFFECT_MAX) g_explosionfxs[index].active = 0;
}

u32 DisplayEffect_GetCount(void) { return s_dispeffect_count; }

void DisplayEffect_Clear(void) { s_dispeffect_count = 0; }
void DisplayEffect_Reset(void) { s_dispeffect_count = 0; }
