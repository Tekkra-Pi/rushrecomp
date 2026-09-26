/* Entity pool functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 *
 * Manages a pool of entity containers using a free-list allocator.
 * The pool provides allocation, deallocation, and type-based querying.
 */

#include "nds_types.h"
#include "entity.h"

/* Pool globals at 0x22b4574 area */
extern EntityContainer g_entity_containers[];
extern u32 g_entity_count;
extern EntityContainer* g_entity_free_list[];
extern u32 g_entity_capacity;

/* ======================================================================== */
/* EntityPool_Init                                                           */
/* Initializes the entity pool by building the free list from the           */
/* container array and resetting the count.                                 */
/* ======================================================================== */
void EntityPool_Init(void) {
    u32 i;

    g_entity_count = 0;

    /* Build free list: all slots start free */
    for (i = 0; i < g_entity_capacity; i++) {
        g_entity_free_list[i] = &g_entity_containers[i];

        /* Clear container */
        g_entity_containers[i].prev = NULL;
        g_entity_containers[i].next = NULL;
        g_entity_containers[i].update = NULL;
        g_entity_containers[i].destroy = NULL;
        g_entity_containers[i].data = NULL;
        g_entity_containers[i].flags = 0;
        g_entity_containers[i].type = 0;
        g_entity_containers[i].size = 0;
        g_entity_containers[i].marker = 0xFFFF;
    }
}

/* ======================================================================== */
/* EntityPool_Free                                                           */
/* Frees all entities in the pool and resets the allocator.                 */
/* ======================================================================== */
void EntityPool_Free(void) {
    u32 i;

    /* Destroy any active entities */
    for (i = 0; i < g_entity_capacity; i++) {
        if (g_entity_containers[i].update != NULL) {
            if (g_entity_containers[i].destroy) {
                g_entity_containers[i].destroy(&g_entity_containers[i]);
            }
            if (g_entity_containers[i].data && g_entity_containers[i].size > 0) {
                heap_free(g_entity_containers[i].data);
            }
        }
    }

    EntityPool_Init();
}

/* ======================================================================== */
/* EntityPool_GetCount                                                       */
/* Returns the number of currently allocated entities.                      */
/* ======================================================================== */
u32 EntityPool_GetCount(void) {
    return g_entity_count;
}

/* ======================================================================== */
/* EntityPool_GetUsed                                                        */
/* Returns the number of used (allocated) entity slots.                     */
/* ======================================================================== */
u32 EntityPool_GetUsed(void) {
    u32 count = 0;
    u32 i;

    for (i = 0; i < g_entity_capacity; i++) {
        if (g_entity_containers[i].update != NULL) {
            count++;
        }
    }
    return count;
}

/* ======================================================================== */
/* EntityPool_GetFree                                                        */
/* Returns the number of free entity slots.                                 */
/* ======================================================================== */
u32 EntityPool_GetFree(void) {
    return g_entity_capacity - g_entity_count;
}

/* ======================================================================== */
/* EntityPool_FindAllByType                                                  */
/* Finds all entities matching a given type mask.                           */
/* Args: r0=type mask to match                                              */
/* Returns: count of matching entities                                      */
/* ======================================================================== */
u32 EntityPool_FindAllByType(void) {
    u32 count = 0;
    u32 i;

    for (i = 0; i < g_entity_capacity; i++) {
        if (g_entity_containers[i].update != NULL &&
            g_entity_containers[i].type != 0) {
            count++;
        }
    }
    return count;
}

/* ======================================================================== */
/* EntityPool_Clear                                                          */
/* Clears/resets the entity pool without freeing data.                      */
/* ======================================================================== */
void EntityPool_Clear(void) {
    u32 i;

    for (i = 0; i < g_entity_capacity; i++) {
        g_entity_containers[i].flags |= ENTITY_FLAG_SKIP_WALK;
        g_entity_containers[i].update = NULL;
        g_entity_containers[i].destroy = NULL;
        g_entity_containers[i].data = NULL;
        g_entity_containers[i].type = 0;
        g_entity_containers[i].size = 0;
    }

    g_entity_count = 0;
}
