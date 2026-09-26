/* Stage/Mode transition system.
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 711-737 */

#include "nds_types.h"
#include "zone_state.h"
#include "entity.h"


/* External globals */
extern ZoneState g_zone_state;
extern GameStateContext g_game_state;
extern u32 g_system_flags;  /* 0x2087cb8 */

void Mode_FadeUpdate(EntityContainer *entity);
void Mode_FadeDestroy(EntityContainer *entity);

/* Stage_SetTransition @ 0x02043c7c (276 bytes)
 * Initiates a stage/mode transition.
 * Allocates a 0x3e00-sized mode entity and starts fade.
 * Args: r0=mode (0x43='C', 0x42='B'), r1=data (0x100) */
void Stage_SetTransition(u32 mode, u32 data) {
    EntityContainer *entity;
    ModeTransitionData *transition;
    
    /* Allocate mode entity */
    entity = Entity_AllocContainer(
        Mode_FadeUpdate,      /* update callback */
        Mode_FadeDestroy,     /* destroy callback */
        0x5000,               /* type */
        0xffff,               /* marker */
        0x3e00                /* data size */
    );
    
    if (!entity) return;
    
    /* Store transition data pointer */
    transition = (ModeTransitionData*)entity->data;
    
    /* Initialize transition */
    transition->flags_00 = 0;
    transition->counter_02 = 0x1100;  /* Initial counter value */
    transition->mode_04 = mode;
    transition->target_06 = data;
    
    /* Store to global */
    g_mode_transition = transition;
    
    /* Check system flags for variant path */
    if (g_system_flags & 0x100) {
        /* Variant path */
        transition->flags_00 |= 0x04;
    }
}

/* Mode_FadeUpdate @ 0x02043b78 (220 bytes)
 * Update callback for mode transition entity.
 * Decrements counter toward target, triggers mode change when done.
 * Args: r0=entity */
void Mode_FadeUpdate(EntityContainer *entity) {
    ModeTransitionData *data = (ModeTransitionData*)entity->data;
    
    if (!data) return;
    
    /* Decrement counter toward target */
    if (data->counter_02 > data->mode_04) {
        data->counter_02--;
    } else {
        /* Counter reached target - trigger mode change */
        data->flags_00 |= 0x20;  /* Set completion flag */
    }
    
    /* Check if entity should be removed */
    if (!(data->flags_00 & 0x04)) {
        /* Remove entity */
        Entity_Remove(entity);
    }
}

/* Mode_FadeDestroy @ 0x02043c68
 * Destroy callback for mode transition entity.
 * Args: r0=entity */
void Mode_FadeDestroy(EntityContainer *entity) {
    /* Cleanup mode transition */
    g_mode_transition = NULL;
}
