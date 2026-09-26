/* Object interaction functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "object_list.h"
#include "player.h"


/* ObjectInteraction_Init @ 0x02012c00 (168 bytes)
 * Initializes object interaction.
 * Args: r0=object */
void ObjectInteraction_Init(void *object) {
    u32 *data = (u32*)object;
    
    /* Clear interaction state */
    data[15] = 0;  /* +0x3c: interaction flags */
    data[16] = 0;  /* +0x40: interaction type */
    data[17] = 0;  /* +0x44: interaction handler */
}

/* ObjectInteraction_SetType @ 0x02012ca8 (232 bytes)
 * Sets object interaction type.
 * Args: r0=object, r1=type */
void ObjectInteraction_SetType(void *object, u32 type) {
    u32 *data = (u32*)object;
    data[16] = type;
}

/* ObjectInteraction_GetType @ 0x02012d90 (296 bytes)
 * Returns object interaction type.
 * Args: r0=object
 * Returns: interaction type */
u32 ObjectInteraction_GetType(void *object) {
    u32 *data = (u32*)object;
    return data[16];
}

/* ObjectInteraction_SetHandler @ 0x02012eb8
 * Sets interaction handler.
 * Args: r0=object, r1=handler */
void ObjectInteraction_SetHandler(void *object, void (*handler)(void*, void*)) {
    u32 *data = (u32*)object;
    data[17] = (u32)handler;
}

/* ObjectInteraction_Check @ 0x02012f00
 * Checks interaction with player.
 * Args: r0=object
 * Returns: 1 if interacting, 0 otherwise */
s32 ObjectInteraction_Check(void *object) {
    u32 *data = (u32*)object;
    PhysicsPlayer *player = (PhysicsPlayer*)Entity_GetPlayer();
    if (!player) return 0;
    
    /* Check distance */
    s32 dx = player->pos_x - data[2];
    s32 dy = player->pos_y - data[3];
    s32 dist_sq = (dx * dx + dy * dy) >> 12;
    
    s32 interact_range = data[18];  /* +0x48: interaction range */
    
    return dist_sq < interact_range * interact_range;
}

/* ObjectInteraction_Execute @ 0x02012fc0
 * Executes object interaction.
 * Args: r0=object, r1=player */
void ObjectInteraction_Execute(void *object, void *player) {
    u32 *data = (u32*)object;
    
    if (data[17]) {
        void (*handler)(void*, void*) = (void(*)(void*, void*))data[17];
        handler(object, player);
    }
}

/* ObjectInteraction_SetRange @ 0x02013020
 * Sets interaction range.
 * Args: r0=object, r1=range */
void ObjectInteraction_SetRange(void *object, s32 range) {
    u32 *data = (u32*)object;
    data[18] = range;
}

/* ObjectInteraction_GetRange @ 0x02013060
 * Returns interaction range.
 * Args: r0=object
 * Returns: range */
s32 ObjectInteraction_GetRange(void *object) {
    u32 *data = (u32*)object;
    return data[18];
}

/* ObjectInteraction_SetFlags @ 0x020130a0
 * Sets interaction flags.
 * Args: r0=object, r1=flags */
void ObjectInteraction_SetFlags(void *object, u32 flags) {
    u32 *data = (u32*)object;
    data[15] = flags;
}

/* ObjectInteraction_GetFlags @ 0x020130e0
 * Returns interaction flags.
 * Args: r0=object
 * Returns: flags */
u32 ObjectInteraction_GetFlags(void *object) {
    u32 *data = (u32*)object;
    return data[15];
}
