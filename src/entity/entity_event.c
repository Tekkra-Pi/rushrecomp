/* Entity event functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "entity.h"

/* EntityEvent_Init @ 0x02011400 (168 bytes)
 * Initializes entity event system.
 * Args: r0=entity */
void EntityEvent_Init(void *entity) {
    u32 *data = (u32*)entity;
    
    /* Clear event state */
    data[8] = 0;   /* +0x20: event flags */
    data[9] = 0;   /* +0x24: event handler */
}

/* EntityEvent_Fire @ 0x020114a8 (232 bytes)
 * Fires entity event.
 * Args: r0=entity, r1=event_id */
void EntityEvent_Fire(void *entity, u32 event_id) {
    u32 *data = (u32*)entity;
    
    /* Set event flag */
    data[8] |= (1 << event_id);
    
    /* Call handler if set */
    if (data[9]) {
        void (*handler)(void*, u32) = (void(*)(void*, u32))data[9];
        handler(entity, event_id);
    }
}

/* EntityEvent_SetHandler @ 0x02011590 (296 bytes)
 * Sets entity event handler.
 * Args: r0=entity, r1=handler */
void EntityEvent_SetHandler(void *entity, void (*handler)(void*, u32)) {
    u32 *data = (u32*)entity;
    data[9] = (u32)handler;
}

/* EntityEvent_Check @ 0x020116b8
 * Checks if event is set.
 * Args: r0=entity, r1=event_id
 * Returns: 1 if set, 0 otherwise */
s32 EntityEvent_Check(void *entity, u32 event_id) {
    u32 *data = (u32*)entity;
    return (data[8] & (1 << event_id)) ? 1 : 0;
}

/* EntityEvent_Clear @ 0x02011700
 * Clears entity event.
 * Args: r0=entity, r1=event_id */
void EntityEvent_Clear(void *entity, u32 event_id) {
    u32 *data = (u32*)entity;
    data[8] &= ~(1 << event_id);
}

/* EntityEvent_ClearAll @ 0x02011740
 * Clears all entity events.
 * Args: r0=entity */
void EntityEvent_ClearAll(void *entity) {
    u32 *data = (u32*)entity;
    data[8] = 0;
}

/* EntityEvent_GetFlags @ 0x02011780
 * Returns entity event flags.
 * Args: r0=entity
 * Returns: event flags */
u32 EntityEvent_GetFlags(void *entity) {
    u32 *data = (u32*)entity;
    return data[8];
}

/* EntityEvent_SetFlags @ 0x020117c0
 * Sets entity event flags.
 * Args: r0=entity, r1=flags */
void EntityEvent_SetFlags(void *entity, u32 flags) {
    u32 *data = (u32*)entity;
    data[8] = flags;
}
