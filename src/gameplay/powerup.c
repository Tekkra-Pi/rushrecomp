#include "nds_types.h"
static u32 s_count;
void PowerUp_Init(void) { s_count = 0; g_powerup_count = 0; }
s32 PowerUp_Spawn(s32 x, s32 y, u32 param) {
    if (s_count >= 8) return -1;
    g_powerups[s_count].x = x; g_powerups[s_count].y = y;
    g_powerups[s_count].type = param; g_powerups[s_count].timer = 0; g_powerups[s_count].active = 1;
    s_count++; g_powerup_count = s_count; return s_count - 1;
}
void PowerUp_Remove(u32 index) { if (index < s_count) g_powerups[index].active = 0; }
s32 PowerUp_Check(void) {
    s32 px, py; Player_GetPosition(&px, &py);
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (!g_powerups[i].active) continue;
        s32 dx = px - g_powerups[i].x, dy = py - g_powerups[i].y;
        if (dx >= -0x400 && dx <= 0x400 && dy >= -0x400 && dy <= 0x400) return (s32)i;
    }
    return -1;
}
void PowerUp_Collect(void) { }
void PowerUp_Update(void) { }
u32 PowerUp_GetType(void) { return s_count > 0 ? g_powerups[0].type : 0; }
s32 PowerUp_IsActive(u32 index) { return index < s_count ? g_powerups[index].active : 0; }
u32 PowerUp_GetPlayerType(void) { return g_poweruphelper_type; }
void PowerUp_SetPlayerType(u32 index, u32 value) { (void)index; g_poweruphelper_type = value; }
u32 PowerUp_GetTimer(void) { return g_poweruphelper_timer; }
void PowerUp_UpdatePlayer(void) { }
u32 PowerUp_GetCount(void) { return s_count; }
void PowerUp_Clear(void) { s_count = 0; g_powerup_count = 0; }
void PowerUp_Reset(void) { PowerUp_Clear(); }
