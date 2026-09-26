#include "nds_types.h"
void LevelWind_Init(void) { g_levelwind_active = 0; g_levelwind_strength = 0; }
void LevelWind_Set(u32 strength) { g_levelwind_active = 1; g_levelwind_strength = strength; }
void LevelWind_Stop(void) { g_levelwind_active = 0; }
void LevelWind_Update(void) { if (g_levelwind_active) Player_SetVX(Player_GetVX() + (s32)g_levelwind_strength); }
s32 LevelWind_IsActive(void) { return g_levelwind_active; }
u32 LevelWind_GetStrength(void) { return g_levelwind_strength; }
void LevelWind_Reset(void) { g_levelwind_active = 0; g_levelwind_strength = 0; }
