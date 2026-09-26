#include "nds_types.h"
void LevelGravity_Init(void) { g_levelgravity_normal = 0x40; }
void LevelGravity_Set(u32 gravity) { g_levelgravity_normal = gravity; }
u32 LevelGravity_Get(void) { return g_levelgravity_normal; }
void LevelGravity_Apply(void) { Player_SetGravity(g_levelgravity_normal); }
void LevelGravity_Reset(void) { g_levelgravity_normal = 0x40; }
