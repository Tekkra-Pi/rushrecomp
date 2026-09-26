#include "nds_types.h"
void GrindRail_Init(void) { g_grindrail_count = 0; }
s32 GrindRail_Add(s32 x1, s32 y1, s32 x2, s32 y2) {
    if (g_grindrail_count >= 32) return -1;
    u32 i = g_grindrail_count;
    g_railsegs[i].x1 = x1; g_railsegs[i].y1 = y1; g_railsegs[i].x2 = x2; g_railsegs[i].y2 = y2;
    g_railsegs[i].active = 1; g_grindrail_count++; return i;
}
void GrindRail_Update(void) { }
void GrindRail_Remove(u32 index) { if (index < g_grindrail_count) g_railsegs[index].active = 0; }
u32 GrindRail_GetCount(void) { return g_grindrail_count; }
void GrindRail_Clear(void) { g_grindrail_count = 0; }
void GrindRail_Reset(void) { g_grindrail_count = 0; }
