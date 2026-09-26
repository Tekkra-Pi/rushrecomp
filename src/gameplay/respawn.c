#include "nds_types.h"
void Respawn_Init(void) { g_respawn_x = 0; g_respawn_y = 0; }
void Respawn_Set(u32 index, u32 value) { (void)index; g_respawn_x = (s32)value; }
void Respawn_Get(void) { }
s32 Respawn_IsActive(u32 index) { (void)index; return 1; }
void Respawn_Clear(void) { g_respawn_x = 0; g_respawn_y = 0; }
s32 Respawn_GetX(u32 index) { (void)index; return g_respawn_x; }
s32 Respawn_GetY(u32 index) { (void)index; return g_respawn_y; }
void Respawn_SetX(u32 index, u32 value) { (void)index; g_respawn_x = (s32)value; }
void Respawn_SetY(u32 index, u32 value) { (void)index; g_respawn_y = (s32)value; }
void Respawn_Reset(void) { g_respawn_x = 0; g_respawn_y = 0; }
