/* Gameplay spawn functions. */
#include "nds_types.h"

void Spawn_Init(void) { g_spawncount = 0; g_spawn_idx = 0; }
s32 Spawn_Queue(void) {
    if (g_spawncount >= 64) return -1;
    g_spawncount++; return g_spawncount - 1;
}
void Spawn_Process(void) { if (g_spawn_idx < g_spawncount) g_spawn_idx++; }
s32 Spawn_CheckRange(void) {
    s32 px, py; Player_GetPosition(&px, &py);
    s32 dx = px - g_camera_x, dy = py - g_camera_y;
    return (dx >= -0x2000 && dx <= 0x2000 && dy >= -0x2000 && dy <= 0x2000) ? 1 : 0;
}
void Spawn_Execute(void) { Spawn_Process(); }
void Spawn_Clear(void) { g_spawncount = 0; g_spawn_idx = 0; }
u32 Spawn_GetCount(void) { return g_spawncount; }
void Spawn_SetActive(u32 index, u32 value) { (void)index; (void)value; }
