#include "nds_types.h"
static u32 s_count;
void SpringActor_Init(void) { s_count = 0; g_springactor_count = 0; }
s32 SpringActor_Add(s32 x, s32 y, u32 param) {
    (void)param;
    if (s_count >= 8) return -1;
    g_springpads[s_count].x = x; g_springpads[s_count].y = y;
    g_springpads[s_count].active = 1;
    s_count++; g_springactor_count = s_count; return s_count - 1;
}
void SpringActor_Update(void) { }
void SpringActor_Trig(void) { }
void SpringActor_Remove(u32 index) { if (index < s_count) g_springpads[index].active = 0; }
s32 SpringActor_GetX(u32 index) { return index < s_count ? g_springpads[index].x : 0; }
s32 SpringActor_GetY(u32 index) { return index < s_count ? g_springpads[index].y : 0; }
u32 SpringActor_GetCount(void) { return s_count; }
s32 SpringActor_IsActive(u32 index) { return index < s_count ? g_springpads[index].active : 0; }
void SpringActor_Clear(void) { s_count = 0; g_springactor_count = 0; }
void SpringActor_Reset(void) { SpringActor_Clear(); }
