#include "nds_types.h"
static u32 s_count;
void InvSparkle_Init(void) { s_count = 0; }
s32 InvSparkle_Spawn(s32 x, s32 y) {
    if (s_count >= 16) return -1;
    g_invsparkles[s_count].x = x + ((s_count * 73) % 48) - 24;
    g_invsparkles[s_count].y = y + ((s_count * 47) % 48) - 24;
    g_invsparkles[s_count].timer = 0; g_invsparkles[s_count].active = 1;
    s_count++; return s_count - 1;
}
void InvSparkle_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_invsparkles[i].active) continue;
        g_invsparkles[i].y -= 0x100; g_invsparkles[i].timer++;
        if (g_invsparkles[i].timer >= 12) g_invsparkles[i].active = 0;
    }
}
void InvSparkle_Draw(void) {
    u32 i;
    for (i = 0; i < s_count; i++)
        if (g_invsparkles[i].active)
            OAM_AddSprite(g_invsparkles[i].x >> 8, g_invsparkles[i].y >> 8, 0x400 + (g_invsparkles[i].timer % 4), 0x3000);
}
void InvSparkle_Remove(u32 index) { if (index < s_count) g_invsparkles[index].active = 0; }
u32 InvSparkle_GetCount(void) { return s_count; }
void InvSparkle_Clear(void) { s_count = 0; }
void InvSparkle_Reset(void) { s_count = 0; }
