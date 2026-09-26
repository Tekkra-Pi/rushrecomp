/* Entity system functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 423-450 */

#include "entity.h"
#include "nds_types.h"

/* Entity container array at 0x22b4574 (0x1c bytes each) */
/* Count at 0x22b4578, cap 0x100 */
/* Free table at 0x22b458c */
extern EntityContainer g_entity_containers[256];
extern u32 g_entity_count;
extern EntityContainer* g_entity_free_table[256];

/* Entity_AllocSlot @ 0x02036e9c (68 bytes)
 * Allocates a free entity container slot.
 * Returns: pointer to allocated container, or NULL if full. */
EntityContainer* Entity_AllocSlot(void) {
    EntityContainer* container;
    
    if (g_entity_count >= 0x100) {
        /* Table full */
        return NULL;
    }
    
    container = g_entity_free_table[g_entity_count];
    g_entity_count++;
    
    return container;
}

/* Entity_WalkAll @ 0x02036e04 (144 bytes)
 * Walks all entities and calls their update functions.
 * Args: r0=entity to start at, r1=terminator
 * Skips if: flags & 1, no fn, or priority check fails */
void Entity_WalkAll(EntityContainer* start, EntityContainer* terminator) {
    EntityContainer* current = start;
    
    while (current != terminator) {
        current = current->next;
        
        /* Skip conditions */
        if (current->flags & 1) continue;
        if (!current->update) continue;
        
        /* Priority/gate check against system flags */
        if (SYS_FLAG_UPDATE_FREEZE) {
            u32 sys_type = current->type & ENTITY_TYPE_PHASE_MASK;
            u32 sys_phase = SYS_FLAG_STATE_CHANGE & ENTITY_TYPE_PHASE_MASK;
            if (!(current->type & ENTITY_TYPE_ALWAYS_UPDATE) && sys_type < sys_phase) {
                continue;
            }
        }
        
        /* Call update function */
        current->update(current);
    }
}

/* Entity_Remove @ 0x020371c8
 * Removes an entity from the list and frees its data.
 * Args: r0=entity to remove */
void Entity_Remove(EntityContainer* entity) {
    /* Call destroy callback if present */
    if (entity->destroy) {
        entity->destroy(entity);
    }
    
    /* Unlink from list */
    if (entity->prev) {
        entity->prev->next = entity->next;
    }
    if (entity->next) {
        entity->next->prev = entity->prev;
    }
    
    /* Free data block if allocated */
    if (entity->data && entity->size > 0) {
        heap_free(entity->data);
    }
    
    /* Mark as free */
    entity->update = NULL;
    entity->destroy = NULL;
    entity->data = NULL;
    entity->flags = 0;
    entity->type = 0;
    entity->size = 0;
}

/* Entity_AllocContainer @ 0x020372d0 (396 bytes)
 * Allocates and initializes an entity container with callbacks.
 * Args: r0=update_cb, r1=destroy_cb, r2=type, r3=marker, stack=size
 * Returns: pointer to initialized container */
EntityContainer* Entity_AllocContainer(
    void (*update_cb)(EntityContainer*),
    void (*destroy_cb)(EntityContainer*),
    u16 type, u16 marker, u16 data_size
) {
    EntityContainer* entity;
    
    /* Allocate container slot */
    entity = Entity_AllocSlot();
    if (!entity) {
        return NULL;
    }
    
    /* Set callbacks and metadata */
    entity->update = update_cb;
    entity->destroy = destroy_cb;
    entity->type = type;
    entity->marker = marker;
    entity->flags = 0;
    
    /* Allocate data block if size specified */
    if (data_size > 0) {
        entity->data = heap_alloc(data_size, 0x2467531);
        if (!entity->data) {
            return NULL;
        }
        entity->size = data_size;
        
        /* Clear data block */
        memset(entity->data, 0, data_size);
    } else {
        entity->data = NULL;
        entity->size = 0;
    }
    
    return entity;
}
