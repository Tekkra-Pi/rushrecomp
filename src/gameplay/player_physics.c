#include "nds_types.h"
void PlayerPhysics_Init(void) { g_playerphysics_max_vy = 0x400; }
void PlayerPhysics_Update(void) { }
void PlayerPhysics_SetGravity(u32 index, u32 value) { (void)index; Player_SetGravity((s32)value); }
s32 PlayerPhysics_GetGravity(void) { return g_levelgravity_normal; }
void PlayerPhysics_SetMaxVY(u32 index, u32 value) { (void)index; g_playerphysics_max_vy = value; }
void PlayerPhysics_Reset(void) { g_playerphysics_max_vy = 0x400; g_levelgravity_normal = 0x40; }
