/* Gameplay level collect helper functions. */
#include "nds_types.h"

void CollectHelper_Init(void) { g_collectibles = 0; }

s32 CollectHelper_Add(s32 x, s32 y, u32 param) {
    if (g_collectibles >= 64) return -1;
    u32 i = g_collectibles;
    g_itemspawns[i].x = x; g_itemspawns[i].y = y;
    g_itemspawns[i].type = param; g_itemspawns[i].active = 1;
    g_collectibles++; return i;
}

void CollectHelper_Update(void) {
    u32 i;
    for (i = 0; i < g_collectibles; i++) {
        if (!g_itemspawns[i].active) continue;
        s32 px, py; Player_GetPosition(&px, &py);
        s32 dx = px - g_itemspawns[i].x, dy = py - g_itemspawns[i].y;
        if (dx >= -0x400 && dx <= 0x400 && dy >= -0x400 && dy <= 0x400)
            g_itemspawns[i].active = 0;
    }
}

void CollectHelper_Remove(u32 index) {
    if (index < g_collectibles) g_itemspawns[index].active = 0;
}
u32 CollectHelper_GetCount(void) { return g_collectibles; }
void CollectHelper_Clear(void) { g_collectibles = 0; }
void CollectHelper_Reset(void) { g_collectibles = 0; }
