#include "nds_types.h"
void RespawnPt_Init(void) { g_checkpoint_count = 0; }
s32 RespawnPt_Add(s32 x, s32 y, u32 param) {
    (void)param;
    if (g_checkpoint_count >= 8) return -1;
    u32 i = g_checkpoint_count;
    g_respawnpts[i].x = x; g_respawnpts[i].y = y; g_respawnpts[i].active = 1;
    g_checkpoint_count++; return i;
}
void RespawnPt_SetCurrent(u32 index, u32 value) {
    (void)value;
    if (index < g_checkpoint_count) { g_respawn_x = g_respawnpts[index].x; g_respawn_y = g_respawnpts[index].y; }
}
s32 RespawnPt_GetX(u32 index) { return index < g_checkpoint_count ? g_respawnpts[index].x : 0; }
s32 RespawnPt_GetY(u32 index) { return index < g_checkpoint_count ? g_respawnpts[index].y : 0; }
void RespawnPt_Remove(u32 index) { if (index < g_checkpoint_count) g_respawnpts[index].active = 0; }
u32 RespawnPt_GetCount(void) { return g_checkpoint_count; }
void RespawnPt_Clear(void) { g_checkpoint_count = 0; }
void RespawnPt_Reset(void) { g_checkpoint_count = 0; }
