#include "nds_types.h"
static u32 s_count;
void SpawnPoint_Init(void) { s_count = 0; }
s32 SpawnPoint_Add(s32 x, s32 y, u32 param) {
    (void)param;
    if (s_count >= 8) return -1;
    g_respawnpts[s_count].x = x; g_respawnpts[s_count].y = y; g_respawnpts[s_count].active = 1;
    s_count++; return s_count - 1;
}
void SpawnPoint_SetCurrent(u32 index, u32 value) {
    (void)value;
    if (index < s_count) { g_respawn_x = g_respawnpts[index].x; g_respawn_y = g_respawnpts[index].y; }
}
s32 SpawnPoint_GetX(u32 index) { return index < s_count ? g_respawnpts[index].x : 0; }
s32 SpawnPoint_GetY(u32 index) { return index < s_count ? g_respawnpts[index].y : 0; }
void SpawnPoint_Remove(u32 index) { if (index < s_count) g_respawnpts[index].active = 0; }
u32 SpawnPoint_GetCount(void) { return s_count; }
void SpawnPoint_Clear(void) { s_count = 0; }
void SpawnPoint_Reset(void) { s_count = 0; }
