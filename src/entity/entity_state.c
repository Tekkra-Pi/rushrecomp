/* Entity state functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 *
 * Manages per-entity state values (current, previous, timer).
 * Uses a global state array indexed by entity ID.
 */

#include "nds_types.h"

/* State table globals */
#define MAX_ENTITIES 256

typedef struct {
    u16 current;        /* +0x00: current state value */
    u16 previous;       /* +0x02: previous state value */
    u16 timer;          /* +0x04: state timer countdown */
    u16 _pad;           /* +0x06: padding */
} EntityStateEntry;

extern EntityStateEntry g_entity_state_table[MAX_ENTITIES];
extern u32 g_entity_state_index;

/* ======================================================================== */
/* EntityState_Init                                                          */
/* Initializes the entity state system by clearing all state entries.       */
/* ======================================================================== */
void EntityState_Init(void) {
    u32 i;
    for (i = 0; i < MAX_ENTITIES; i++) {
        g_entity_state_table[i].current = 0;
        g_entity_state_table[i].previous = 0;
        g_entity_state_table[i].timer = 0;
    }
    g_entity_state_index = 0;
}

/* ======================================================================== */
/* EntityState_Set                                                           */
/* Sets the current state value for the indexed entity.                     */
/* Saves the old current as previous before overwriting.                    */
/* Args: r0=index, r1=value                                                  */
/* ======================================================================== */
void EntityState_Set(u32 index, u32 value) {
    if (index < MAX_ENTITIES) {
        g_entity_state_table[index].previous = g_entity_state_table[index].current;
        g_entity_state_table[index].current = (u16)value;
    }
}

/* ======================================================================== */
/* EntityState_Get                                                           */
/* Returns the current state value for the indexed entity.                  */
/* ======================================================================== */
u32 EntityState_Get(void) {
    u32 idx = g_entity_state_index;
    if (idx < MAX_ENTITIES) {
        return g_entity_state_table[idx].current;
    }
    return 0;
}

/* ======================================================================== */
/* EntityState_GetPrevious                                                   */
/* Returns the previous state value for the indexed entity.                 */
/* ======================================================================== */
u32 EntityState_GetPrevious(void) {
    u32 idx = g_entity_state_index;
    if (idx < MAX_ENTITIES) {
        return g_entity_state_table[idx].previous;
    }
    return 0;
}

/* ======================================================================== */
/* EntityState_GetTimer                                                      */
/* Returns the state timer value for the indexed entity.                    */
/* ======================================================================== */
u32 EntityState_GetTimer(void) {
    u32 idx = g_entity_state_index;
    if (idx < MAX_ENTITIES) {
        return g_entity_state_table[idx].timer;
    }
    return 0;
}

/* ======================================================================== */
/* EntityState_Update                                                        */
/* Per-frame update: decrements timers for all active entities.            */
/* ======================================================================== */
void EntityState_Update(void) {
    u32 i;
    for (i = 0; i < MAX_ENTITIES; i++) {
        if (g_entity_state_table[i].timer > 0) {
            g_entity_state_table[i].timer--;
        }
    }
}

/* ======================================================================== */
/* EntityState_Is                                                            */
/* Returns 1 if the current state equals the given value.                   */
/* ======================================================================== */
s32 EntityState_Is(void) {
    u32 idx = g_entity_state_index;
    if (idx < MAX_ENTITIES) {
        return g_entity_state_table[idx].current == 1 ? 1 : 0;
    }
    return 0;
}

/* ======================================================================== */
/* EntityState_IsPrevious                                                    */
/* Returns 1 if the previous state equals the given value.                  */
/* ======================================================================== */
s32 EntityState_IsPrevious(void) {
    u32 idx = g_entity_state_index;
    if (idx < MAX_ENTITIES) {
        return g_entity_state_table[idx].previous == 1 ? 1 : 0;
    }
    return 0;
}

/* ======================================================================== */
/* EntityState_IsTimer                                                       */
/* Returns 1 if the state timer is non-zero (timer active).                */
/* ======================================================================== */
s32 EntityState_IsTimer(void) {
    u32 idx = g_entity_state_index;
    if (idx < MAX_ENTITIES) {
        return g_entity_state_table[idx].timer != 0 ? 1 : 0;
    }
    return 0;
}

/* ======================================================================== */
/* EntityState_Reset                                                         */
/* Resets the current state, previous state, and timer for all entities.   */
/* ======================================================================== */
void EntityState_Reset(void) {
    u32 i;
    for (i = 0; i < MAX_ENTITIES; i++) {
        g_entity_state_table[i].current = 0;
        g_entity_state_table[i].previous = 0;
        g_entity_state_table[i].timer = 0;
    }
}
