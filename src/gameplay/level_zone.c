#include "nds_types.h"
void LevelZone_Init(void) { g_current_zone = 0; g_current_act = 0; }
void LevelZone_Set(u32 zone, u32 act) { g_current_zone = zone; g_current_act = act; }
u32 LevelZone_GetZone(void) { return g_current_zone; }
u32 LevelZone_GetAct(void) { return g_current_act; }
void LevelZone_Load(void) { ZoneMgr_LoadZone(g_current_zone, g_current_act); }
void LevelZone_Reset(void) { g_current_zone = 0; g_current_act = 0; }
