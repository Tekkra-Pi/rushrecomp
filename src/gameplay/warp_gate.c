#include "nds_types.h"
static u32 s_count;
void WarpGate_Init(void) { s_count = 0; g_warpgate_count = 0; }
s32 WarpGate_Add(s32 x, s32 y, u32 param) {
    (void)param;
    if (s_count >= 8) return -1;
    g_warpentries[s_count].x = x; g_warpentries[s_count].y = y;
    g_warpentries[s_count].active = 1;
    s_count++; g_warpgate_count = s_count; return s_count - 1;
}
void WarpGate_Update(void) { }
void WarpGate_Remove(u32 index) { if (index < s_count) g_warpentries[index].active = 0; }
u32 WarpGate_GetCount(void) { return s_count; }
void WarpGate_Clear(void) { s_count = 0; g_warpgate_count = 0; }
void WarpGate_Reset(void) { WarpGate_Clear(); }
