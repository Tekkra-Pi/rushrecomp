#include "nds_types.h"
void LevelWarp_Init(void) { g_levelwarp_dest_zone = 0; g_levelwarp_dest_x = 0; g_levelwarp_dest_y = 0; }
void LevelWarp_Set(u32 zone, s32 x, s32 y) { g_levelwarp_dest_zone = zone; g_levelwarp_dest_x = x; g_levelwarp_dest_y = y; }
u32 LevelWarp_GetZone(void) { return g_levelwarp_dest_zone; }
s32 LevelWarp_GetX(void) { return g_levelwarp_dest_x; }
s32 LevelWarp_GetY(void) { return g_levelwarp_dest_y; }
void LevelWarp_Reset(void) { LevelWarp_Init(); }
