/* Object spawn and initialization functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern ObjSpawnerEntry g_objspawners[];
extern u32 g_objspawner_count;

#define MAX_OBJ_SPAWNERS 64

u32 ObjSpawn_FromTable(void) {
    u32 i, count = 0;
    for (i = 0; i < g_objspawner_count; i++) {
        if (g_objspawners[i].active && !g_objspawners[i].spawned) {
            extern void Object_Spawn(s32 x, s32 y, u32 type);
            Object_Spawn(g_objspawners[i].x, g_objspawners[i].y,
                         g_objspawners[i].type);
            g_objspawners[i].spawned = 1;
            count++;
        }
    }
    return count;
}

void ObjSpawn_Queue(void) {
    /* Queue pending spawns for next frame */
    u32 i;
    for (i = 0; i < g_objspawner_count; i++) {
        if (g_objspawners[i].active && !g_objspawners[i].spawned) {
            /* Mark for deferred spawn */
        }
    }
}
