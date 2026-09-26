#include "nds_types.h"
void LevelCheckpointData_Init(void) { g_checkpoint_count = 0; }
s32 LevelCheckpointData_Add(s32 x, s32 y) {
    if (g_checkpoint_count >= 8) return -1;
    u32 i = g_checkpoint_count;
    g_respawnpts[i].x = x; g_respawnpts[i].y = y; g_respawnpts[i].active = 1;
    g_checkpoint_count++; return i;
}
void LevelCheckpointData_SetCurrent(u32 index) {
    if (index < g_checkpoint_count) { g_respawn_x = g_respawnpts[index].x; g_respawn_y = g_respawnpts[index].y; }
}
void LevelCheckpointData_Remove(u32 index) { if (index < g_checkpoint_count) g_respawnpts[index].active = 0; }
u32 LevelCheckpointData_GetCount(void) { return g_checkpoint_count; }
void LevelCheckpointData_Clear(void) { g_checkpoint_count = 0; }
void LevelCheckpointData_Reset(void) { g_checkpoint_count = 0; }
