#include "nds_types.h"
static u32 s_count;
void RollTrail_Init(void) { s_count = 0; }
s32 RollTrail_Spawn(s32 x, s32 y, u32 param) {
    (void)param;
    if (s_count >= 16) return -1;
    g_rolltrails[s_count].x = x; g_rolltrails[s_count].y = y;
    g_rolltrails[s_count].timer = 0; g_rolltrails[s_count].active = 1;
    s_count++; return s_count - 1;
}
void RollTrail_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_rolltrails[i].active) continue;
        g_rolltrails[i].timer++;
        if (g_rolltrails[i].timer >= 8) g_rolltrails[i].active = 0;
    }
}
void RollTrail_Draw(void) {
    u32 i;
    for (i = 0; i < s_count; i++)
        if (g_rolltrails[i].active)
            OAM_AddSprite(g_rolltrails[i].x >> 8, g_rolltrails[i].y >> 8, 0x300, 0x2000);
}
void RollTrail_Remove(u32 index) { if (index < s_count) g_rolltrails[index].active = 0; }
u32 RollTrail_GetCount(void) { return s_count; }
s32 RollTrail_IsActive(u32 index) { return index < s_count ? g_rolltrails[index].active : 0; }
void RollTrail_Clear(void) { s_count = 0; }
