#include "nds_types.h"
static u32 s_count;
void TrickSparkle_Init(void) { s_count = 0; }
s32 TrickSparkle_Spawn(s32 x, s32 y, u32 param) {
    (void)param;
    if (s_count >= 16) return -1;
    g_tricksparkles[s_count].x = x + ((s_count * 53) % 32) - 16;
    g_tricksparkles[s_count].y = y + ((s_count * 37) % 32) - 16;
    g_tricksparkles[s_count].timer = 0; g_tricksparkles[s_count].active = 1;
    s_count++; return s_count - 1;
}
void TrickSparkle_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_tricksparkles[i].active) continue;
        g_tricksparkles[i].y -= 0x100; g_tricksparkles[i].timer++;
        if (g_tricksparkles[i].timer >= 12) g_tricksparkles[i].active = 0;
    }
}
void TrickSparkle_Draw(void) {
    u32 i;
    for (i = 0; i < s_count; i++)
        if (g_tricksparkles[i].active)
            OAM_AddSprite(g_tricksparkles[i].x >> 8, g_tricksparkles[i].y >> 8, 0x400 + (g_tricksparkles[i].timer % 4), 0x3000);
}
void TrickSparkle_Remove(u32 index) { if (index < s_count) g_tricksparkles[index].active = 0; }
u32 TrickSparkle_GetCount(void) { return s_count; }
s32 TrickSparkle_IsActive(u32 index) { return index < s_count ? g_tricksparkles[index].active : 0; }
u32 TrickSparkle_GetType(void) { return 0; }
void TrickSparkle_SpawnBurst(void) {
    s32 px, py; Player_GetPosition(&px, &py);
    u32 i; for (i = 0; i < 4; i++) TrickSparkle_Spawn(px, py, 0);
}
void TrickSparkle_Clear(void) { s_count = 0; }
