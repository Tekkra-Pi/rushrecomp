#include "nds_types.h"
void WaterEffect_Init(void) { g_levelwater_active = 0; g_levelwater_level = 0; }
void WaterEffect_Start(void) { g_levelwater_active = 1; }
void WaterEffect_Stop(void) { g_levelwater_active = 0; }
void WaterEffect_Update(void) { }
s32 WaterEffect_IsActive(u32 index) { (void)index; return g_levelwater_active; }
s32 WaterEffect_GetLevel(void) { return g_levelwater_level; }
void WaterEffect_Reset(void) { g_levelwater_active = 0; g_levelwater_level = 0; }
