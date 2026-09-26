/* Gameplay zone functions. */
#include "nds_types.h"

void GameZone_Init(void) { g_current_zone = 0; g_current_act = 0; }
void GameZone_Set(u32 index, u32 value) { (void)index; g_current_zone = value; }
void GameZone_LoadData(void) { }
u32 GameZone_GetFlags(void) { return g_zone_flags; }
void GameZone_SetFlag(u32 index, u32 value) { (void)index; g_zone_flags = value; }
void GameZone_ClearFlag(void) { g_zone_flags = 0; }
s32 GameZone_CheckFlag(void) { return g_zone_flags != 0 ? 1 : 0; }
