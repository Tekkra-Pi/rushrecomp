/* Collision dispatch functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* External globals */
extern CollDispatchEntry g_colldispatchs[];
extern u32 collision_entity_count;
extern void *collision_entity_list;

/* Maximum dispatch table entries */
#define COLL_DISPATCH_MAX 64

/* CollisionDispatch_Init
 * Initializes the collision dispatch table.
 * Args: none */
void CollisionDispatch_Init(void) {
    /* Clear all dispatch entries */
    for (u32 i = 0; i < COLL_DISPATCH_MAX; i++) {
        g_colldispatchs[i].type_a = 0;
        g_colldispatchs[i].type_b = 0;
        g_colldispatchs[i].callback = NULL;
        g_colldispatchs[i].active = 0;
    }
}

/* CollisionDispatch_RemovePair
 * Removes a collision pair from the dispatch table.
 * Args: r0=type_a, r1=type_b */
void CollisionDispatch_RemovePair(u32 type_a, u32 type_b) {
    for (u32 i = 0; i < COLL_DISPATCH_MAX; i++) {
        if (g_colldispatchs[i].active &&
            g_colldispatchs[i].type_a == type_a &&
            g_colldispatchs[i].type_b == type_b) {
            g_colldispatchs[i].active = 0;
            g_colldispatchs[i].callback = NULL;
            return;
        }
    }
}

/* CollisionDispatch_Process
 * Processes all active collision pairs in the dispatch table.
 * Args: none */
void CollisionDispatch_Process(void) {
    u32 count = collision_entity_count;
    void **list = (void **)&collision_entity_list;

    for (u32 i = 0; i < COLL_DISPATCH_MAX; i++) {
        if (!g_colldispatchs[i].active || g_colldispatchs[i].callback == NULL) {
            continue;
        }

        /* Test all entity pairs against this dispatch entry */
        for (u32 a = 0; a < count; a++) {
            void *ent_a = list[a];
            if (ent_a == NULL) continue;

            void *data_a = *(void **)ent_a;
            if (data_a == NULL) continue;

            for (u32 b = a + 1; b < count; b++) {
                void *ent_b = list[b];
                if (ent_b == NULL) continue;

                void *data_b = *(void **)ent_b;
                if (data_b == NULL) continue;

                /* Get entity types */
                u32 type_a = *((u32 *)data_a + 6);  /* +0x18: type */
                u32 type_b = *((u32 *)data_b + 6);  /* +0x18: type */

                /* Check if types match dispatch entry */
                if ((type_a == g_colldispatchs[i].type_a &&
                     type_b == g_colldispatchs[i].type_b) ||
                    (type_a == g_colldispatchs[i].type_b &&
                     type_b == g_colldispatchs[i].type_a)) {
                    /* AABB overlap check */
                    s32 ax = *((s32 *)data_a + 4) >> 8;
                    s32 ay = *((s32 *)data_a + 5) >> 8;
                    u16 aw = *((u16 *)ent_a + 16);
                    u16 ah = *((u16 *)ent_a + 17);

                    s32 bx = *((s32 *)data_b + 4) >> 8;
                    s32 by = *((s32 *)data_b + 5) >> 8;
                    u16 bw = *((u16 *)ent_b + 16);
                    u16 bh = *((u16 *)ent_b + 17);

                    if (ax < bx + (s32)bw && ax + (s32)aw > bx &&
                        ay < by + (s32)bh && ay + (s32)ah > by) {
                        /* Collision detected - invoke callback */
                        g_colldispatchs[i].callback(ent_a);
                    }
                }
            }
        }
    }
}

/* CollisionDispatch_Clear
 * Clears all active collision pairs.
 * Args: none */
void CollisionDispatch_Clear(void) {
    for (u32 i = 0; i < COLL_DISPATCH_MAX; i++) {
        g_colldispatchs[i].active = 0;
        g_colldispatchs[i].callback = NULL;
    }
}
