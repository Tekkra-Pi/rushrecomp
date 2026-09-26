#include "nds_types.h"
static u32 s_count;
void MomentumZone_Init(void) { s_count = 0; g_momentumzone_count = 0; }
s32 MomentumZone_Add(s32 x, s32 y, s32 w, s32 h) {
    if (s_count >= 8) return -1;
    g_gravzones[s_count].x = x; g_gravzones[s_count].y = y;
    g_gravzones[s_count].w = w; g_gravzones[s_count].h = h;
    g_gravzones[s_count].gravity = 0x40; g_gravzones[s_count].active = 1;
    s_count++; g_momentumzone_count = s_count; return s_count - 1;
}
void MomentumZone_Update(void) { }
void MomentumZone_Remove(u32 index) { if (index < s_count) g_gravzones[index].active = 0; }
u32 MomentumZone_GetCount(void) { return s_count; }
void MomentumZone_Clear(void) { s_count = 0; g_momentumzone_count = 0; }
void MomentumZone_Reset(void) { MomentumZone_Clear(); }
