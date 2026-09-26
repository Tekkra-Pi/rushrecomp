/* Gameplay level zone manager functions. */
#include "nds_types.h"


void ZoneMgr_Init(void) { g_zonemgr_zone = 0; g_zonemgr_act = 0; g_zonemgr_loaded = 0; }

void ZoneMgr_LoadZone(u32 zone, u32 act) {
    g_zonemgr_zone = zone; g_zonemgr_act = act; g_zonemgr_loaded = 1;
    ChunkLoader_Load(zone, act);
    Parallax_Init();
    LevelBoundsHelper_Init();
    Enemy_Init();
    Object_Init();
}

void ZoneMgr_SetZone(u32 zone, u32 act) { g_zonemgr_zone = zone; g_zonemgr_act = act; }
u32 ZoneMgr_GetZone(void) { return g_zonemgr_zone; }
u32 ZoneMgr_GetAct(void) { return g_zonemgr_act; }
s32 ZoneMgr_IsLoaded(void) { return g_zonemgr_loaded; }
void ZoneMgr_Reset(void) { g_zonemgr_zone = 0; g_zonemgr_act = 0; g_zonemgr_loaded = 0; }
