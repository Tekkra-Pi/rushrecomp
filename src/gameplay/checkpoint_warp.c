/* Gameplay level checkpoint warp functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);

#define CHECKPOINTWARP_MAX 16

void CheckpointWarp_Init(void) { }

s32 CheckpointWarp_Add(s32 x, s32 y, u32 param) {
    u32 i = g_cpwrap_count;
    g_cpwraps[i].x = x;
    g_cpwraps[i].y = y;
    g_cpwraps[i].dest_x = param;
    g_cpwraps[i].dest_y = 0;
    g_cpwraps[i].active = 1;
    g_cpwrap_count++;
    return i;
}

void CheckpointWarp_Update(void) {
    u32 i;
    for (i = 0; i < g_cpwrap_count; i++) {
        if (!g_cpwraps[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - g_cpwraps[i].x;
        s32 dy = py - g_cpwraps[i].y;
        if (dx >= -0x200 && dx <= 0x200 && dy >= -0x400 && dy <= 0x400) {
            g_levelwarp_dest_x = g_cpwraps[i].dest_x;
            g_levelwarp_dest_y = g_cpwraps[i].dest_y;
        }
    }
}

void CheckpointWarp_Remove(u32 index) {
    if (index < g_cpwrap_count) g_cpwraps[index].active = 0;
}

s32 CheckpointWarp_GetDestX(void) {
    return g_levelwarp_dest_x;
}

s32 CheckpointWarp_GetDestY(void) {
    return g_levelwarp_dest_y;
}

u32 CheckpointWarp_GetDestZone(void) {
    return g_levelwarp_dest_zone;
}

u32 CheckpointWarp_GetCount(void) { return g_cpwrap_count; }

s32 CheckpointWarp_IsActive(u32 index) {
    if (index >= g_cpwrap_count) return 0;
    return g_cpwraps[index].active;
}

void CheckpointWarp_Clear(void) { g_cpwrap_count = 0; }
void CheckpointWarp_Reset(void) { g_cpwrap_count = 0; }
