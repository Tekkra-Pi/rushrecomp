#include "nds_types.h"
static u32 s_count;
void GrindSpark_Init(void) { s_count = 0; }
s32 GrindSpark_Spawn(s32 x, s32 y, u32 param) {
    if (s_count >= 8) return -1;
    g_grindsparks[s_count].x = x; g_grindsparks[s_count].y = y;
    g_grindsparks[s_count].vel_x = (param & 1) ? -0x80 : 0x80;
    g_grindsparks[s_count].vel_y = -0x100;
    g_grindsparks[s_count].timer = 0; g_grindsparks[s_count].active = 1;
    s_count++; return s_count - 1;
}
void GrindSpark_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_grindsparks[i].active) continue;
        g_grindsparks[i].x += g_grindsparks[i].vel_x; g_grindsparks[i].y += g_grindsparks[i].vel_y;
        g_grindsparks[i].vel_y += 0x20; g_grindsparks[i].timer++;
        if (g_grindsparks[i].timer >= 16) g_grindsparks[i].active = 0;
    }
}
void GrindSpark_Draw(void) {
    u32 i;
    for (i = 0; i < s_count; i++)
        if (g_grindsparks[i].active)
            OAM_AddSprite(g_grindsparks[i].x >> 8, g_grindsparks[i].y >> 8, 0x300, 0x2000);
}
void GrindSpark_Remove(u32 index) { if (index < s_count) g_grindsparks[index].active = 0; }
u32 GrindSpark_GetCount(void) { return s_count; }
s32 GrindSpark_IsActive(u32 index) { return index < s_count ? g_grindsparks[index].active : 0; }
void GrindSpark_SpawnBurst(void) {
    s32 px, py; Player_GetPosition(&px, &py);
    u32 i; for (i = 0; i < 4; i++) GrindSpark_Spawn(px, py + 0x200, i & 1);
}
void GrindSpark_Clear(void) { s_count = 0; }
