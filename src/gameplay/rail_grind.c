#include "nds_types.h"
void RailGrind_Init(void) { g_grindrail_count = 0; }
void RailGrind_Update(void) { }
void RailGrind_End(void) { }
s32 RailGrind_CheckEntry(void) {
    s32 px, py; Player_GetPosition(&px, &py);
    u32 i;
    for (i = 0; i < g_grindrail_count; i++) {
        if (!g_railsegs[i].active) continue;
        s32 dx = px - g_railsegs[i].x1, dy = py - g_railsegs[i].y1;
        if (dx >= -0x200 && dx <= 0x200 && dy >= -0x200 && dy <= 0x200) return (s32)i;
    }
    return -1;
}
s32 RailGrind_GetAngle(void) { return 0; }
