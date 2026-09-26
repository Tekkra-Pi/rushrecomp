#include "nds_types.h"
static u32 s_count;
void TrailFX_Init(void) { s_count = 0; g_trailfx_count = 0; }
s32 TrailFX_Add(s32 x, s32 y, u32 param) {
    if (s_count >= 32) return -1;
    g_trailfxs[s_count].x = x; g_trailfxs[s_count].y = y;
    g_trailfxs[s_count].tile = param; g_trailfxs[s_count].timer = 0;
    g_trailfxs[s_count].active = 1;
    s_count++; g_trailfx_count = s_count; return s_count - 1;
}
void TrailFX_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_trailfxs[i].active) continue;
        g_trailfxs[i].timer++;
        if (g_trailfxs[i].timer >= 8) g_trailfxs[i].active = 0;
    }
}
void TrailFX_Draw(void) {
    u32 i;
    for (i = 0; i < s_count; i++)
        if (g_trailfxs[i].active)
            OAM_AddSprite(g_trailfxs[i].x >> 8, g_trailfxs[i].y >> 8, g_trailfxs[i].tile, 0x2000);
}
void TrailFX_Remove(u32 index) { if (index < s_count) g_trailfxs[index].active = 0; }
u32 TrailFX_GetCount(void) { return s_count; }
void TrailFX_Clear(void) { s_count = 0; g_trailfx_count = 0; }
void TrailFX_Reset(void) { TrailFX_Clear(); }
