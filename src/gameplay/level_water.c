/* Gameplay level level water functions. */
#include "nds_types.h"


void LevelWater_Init(void) { g_levelwater_active = 0; g_levelwater_level = 0; }

void LevelWater_Start(s32 level) { g_levelwater_active = 1; g_levelwater_level = level; }
void LevelWater_Stop(void) { g_levelwater_active = 0; }

void LevelWater_Update(void) {
    if (!g_levelwater_active) return;
    s32 px, py;
    Player_GetPosition(&px, &py);
    if (py >= g_levelwater_level) {
        Player_SetUnderwater(1);
        Player_SetGravity(0x800);
    } else {
        Player_SetUnderwater(0);
        Player_SetGravity(0x1000);
    }
}

s32 LevelWater_IsActive(void) { return g_levelwater_active; }
s32 LevelWater_GetLevel(void) { return g_levelwater_level; }
void LevelWater_Reset(void) { g_levelwater_active = 0; g_levelwater_level = 0; }
