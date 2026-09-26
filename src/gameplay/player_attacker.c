#include "nds_types.h"
static u32 s_active;
void PlayerAttacker_Init(void) { s_active = 0; }
void PlayerAttacker_Update(void) {
    if (!s_active) return;
    s32 px, py; Player_GetPosition(&px, &py);
    g_playerattacker_x = px; g_playerattacker_y = py;
}
s32 PlayerAttacker_IsActive(u32 index) { (void)index; return s_active; }
s32 PlayerAttacker_GetX(u32 index) { (void)index; return g_playerattacker_x; }
s32 PlayerAttacker_GetY(u32 index) { (void)index; return g_playerattacker_y; }
s32 PlayerAttacker_GetRadius(void) { return 0x300; }
s32 PlayerAttacker_TestHit(void) {
    if (!s_active) return 0;
    u32 i;
    for (i = 0; i < g_enemy_count; i++) {
        if (!g_enemies[i].active) continue;
        s32 dx = g_playerattacker_x - g_enemies[i].x, dy = g_playerattacker_y - g_enemies[i].y;
        if (dx >= -0x300 && dx <= 0x300 && dy >= -0x300 && dy <= 0x300) { Enemy_Kill(i); return 1; }
    }
    return 0;
}
void PlayerAttacker_Reset(void) { s_active = 0; }
