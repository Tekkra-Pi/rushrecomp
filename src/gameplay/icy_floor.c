#include "nds_types.h"
static u32 s_count;
void IcyFloor_Init(void) { s_count = 0; }
s32 IcyFloor_Add(s32 x, s32 y, s32 w) { (void)w;
    if (s_count >= 16) return -1;
    g_icebreaks[s_count].x = x; g_icebreaks[s_count].y = y; g_icebreaks[s_count].active = 1;
    s_count++; return s_count - 1;
}
void IcyFloor_Update(void) { }
void IcyFloor_Remove(u32 index) { if (index < s_count) g_icebreaks[index].active = 0; }
u32 IcyFloor_GetCount(void) { return s_count; }
void IcyFloor_Clear(void) { s_count = 0; }
void IcyFloor_Reset(void) { s_count = 0; }
