/* Action spawner and object creation functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 595-666 */

#include "nds_types.h"

extern ObjSpawnerEntry g_objspawners[];
extern u32 g_objspawner_count;

#define MAX_OBJ_SPAWNERS 64

void Action_ChildLinking(void) {
    u32 i;
    for (i = 0; i < g_objspawner_count; i++) {
        if (g_objspawners[i].active && !g_objspawners[i].spawned) {
            extern void Object_Spawn(s32 x, s32 y, u32 type);
            Object_Spawn(g_objspawners[i].x, g_objspawners[i].y,
                         g_objspawners[i].type);
            g_objspawners[i].spawned = 1;
        }
    }
}
