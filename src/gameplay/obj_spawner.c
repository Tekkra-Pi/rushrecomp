/* Gameplay level object spawner functions. */
#include "nds_types.h"


void ObjSpawner_Init(void) { g_objspawner_count = 0; }

s32 ObjSpawner_Add(s32 x, s32 y, u32 type, s32 range) {
    if (g_objspawner_count >= 16) return -1;
    u32 i = g_objspawner_count;
    g_objspawners[i].x = x; g_objspawners[i].y = y;
    g_objspawners[i].type = type; g_objspawners[i].range = range;
    g_objspawners[i].spawned = 0; g_objspawners[i].active = 1;
    g_objspawner_count++; return i;
}

void ObjSpawner_Update(void) {
    u32 i;
    for (i = 0; i < g_objspawner_count; i++) {
        if (!g_objspawners[i].active || g_objspawners[i].spawned) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_objspawners[i].x;
        if (dx >= -g_objspawners[i].range && dx <= g_objspawners[i].range) {
            Object_Spawn(g_objspawners[i].x, g_objspawners[i].y, g_objspawners[i].type);
            g_objspawners[i].spawned = 1;
        }
    }
}

void ObjSpawner_Remove(u32 i) { if (i < g_objspawner_count) g_objspawner_count--; }
u32 ObjSpawner_GetCount(void) { return g_objspawner_count; }
void ObjSpawner_Clear(void) { g_objspawner_count = 0; }
void ObjSpawner_Reset(void) { g_objspawner_count = 0; }
