#include "nds_types.h"
static u32 s_count;
void LavaHazard_Init(void) { s_count = 0; }
s32 LavaHazard_Add(s32 x, s32 y, s32 w, s32 h, s32 speed) {
    if (s_count >= 8) return -1;
    g_lavas[s_count].x = x; g_lavas[s_count].y = y; g_lavas[s_count].w = w; g_lavas[s_count].h = h;
    g_lavas[s_count].speed = speed; g_lavas[s_count].timer = 0; g_lavas[s_count].active = 1;
    s_count++; return s_count - 1;
}
void LavaHazard_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_lavas[i].active) continue;
        g_lavas[i].timer++; g_lavas[i].x += g_lavas[i].speed;
    }
}
void LavaHazard_Remove(u32 index) { if (index < s_count) g_lavas[index].active = 0; }
u32 LavaHazard_GetCount(void) { return s_count; }
void LavaHazard_Clear(void) { s_count = 0; }
void LavaHazard_Reset(void) { s_count = 0; }
