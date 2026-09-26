#include "nds_types.h"
static u32 s_count;
void RailGrindSpark_Init(void) { s_count = 0; }
s32 RailGrindSpark_Spawn(s32 x, s32 y, u32 param) {
    if (s_count >= 8) return -1;
    g_railgrindsparks[s_count].x = x; g_railgrindsparks[s_count].y = y;
    g_railgrindsparks[s_count].vel_x = (param & 1) ? -0x80 : 0x80;
    g_railgrindsparks[s_count].vel_y = -0x100;
    g_railgrindsparks[s_count].timer = 0; g_railgrindsparks[s_count].active = 1;
    s_count++; return s_count - 1;
}
void RailGrindSpark_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_railgrindsparks[i].active) continue;
        g_railgrindsparks[i].x += g_railgrindsparks[i].vel_x;
        g_railgrindsparks[i].y += g_railgrindsparks[i].vel_y;
        g_railgrindsparks[i].vel_y += 0x20; g_railgrindsparks[i].timer++;
        if (g_railgrindsparks[i].timer >= 16) g_railgrindsparks[i].active = 0;
    }
}
void RailGrindSpark_Draw(void) {
    u32 i;
    for (i = 0; i < s_count; i++)
        if (g_railgrindsparks[i].active)
            OAM_AddSprite(g_railgrindsparks[i].x >> 8, g_railgrindsparks[i].y >> 8, 0x300, 0x2000);
}
void RailGrindSpark_Remove(u32 index) { if (index < s_count) g_railgrindsparks[index].active = 0; }
u32 RailGrindSpark_GetCount(void) { return s_count; }
s32 RailGrindSpark_IsActive(u32 index) { return index < s_count ? g_railgrindsparks[index].active : 0; }
void RailGrindSpark_SpawnBurst(void) {
    s32 px, py; Player_GetPosition(&px, &py);
    u32 i; for (i = 0; i < 4; i++) RailGrindSpark_Spawn(px, py + 0x200, i & 1);
}
void RailGrindSpark_Clear(void) { s_count = 0; }
