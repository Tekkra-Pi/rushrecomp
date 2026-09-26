#include "nds_types.h"
static u32 s_count;
void StickyWall_Init(void) { s_count = 0; g_stickywall_count = 0; }
s32 StickyWall_Add(s32 x, s32 y, u32 param) {
    (void)param;
    if (s_count >= 8) return -1;
    g_stonebreaks[s_count].x = x; g_stonebreaks[s_count].y = y; g_stonebreaks[s_count].active = 1;
    s_count++; g_stickywall_count = s_count; return s_count - 1;
}
void StickyWall_Update(void) { }
void StickyWall_Remove(u32 index) { if (index < s_count) g_stonebreaks[index].active = 0; }
u32 StickyWall_GetCount(void) { return s_count; }
void StickyWall_Clear(void) { s_count = 0; g_stickywall_count = 0; }
void StickyWall_Reset(void) { StickyWall_Clear(); }
