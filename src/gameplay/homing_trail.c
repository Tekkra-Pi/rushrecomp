#include "nds_types.h"
static u32 s_count;
void HomingTrail_Init(void) { s_count = 0; }
s32 HomingTrail_Spawn(s32 x, s32 y) {
    if (s_count >= 16) return -1;
    g_homingtrails[s_count].x = x; g_homingtrails[s_count].y = y;
    g_homingtrails[s_count].timer = 0; g_homingtrails[s_count].active = 1;
    s_count++; return s_count - 1;
}
void HomingTrail_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_homingtrails[i].active) continue;
        g_homingtrails[i].timer++;
        if (g_homingtrails[i].timer >= 8) g_homingtrails[i].active = 0;
    }
}
void HomingTrail_Draw(void) {
    u32 i;
    for (i = 0; i < s_count; i++)
        if (g_homingtrails[i].active)
            OAM_AddSprite(g_homingtrails[i].x >> 8, g_homingtrails[i].y >> 8, 0x400, 0x3000);
}
void HomingTrail_Remove(u32 index) { if (index < s_count) g_homingtrails[index].active = 0; }
u32 HomingTrail_GetCount(void) { return s_count; }
void HomingTrail_Clear(void) { s_count = 0; }
void HomingTrail_Reset(void) { s_count = 0; }
