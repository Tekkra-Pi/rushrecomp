/* Hitbox collision functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "player.h"

/* Hitbox_Init @ 0x0206f800 (192 bytes)
 * Initializes a hitbox.
 * Args: r0=hitbox_data, r1=offset_x, r2=offset_y, r3=width, sp=height, sp+4=damage */
void Hitbox_Init(void *hitbox_data, s32 offset_x, s32 offset_y, s32 width, s32 height, u32 damage) {
    s32 *data = (s32*)hitbox_data;
    
    /* Set hitbox parameters */
    data[0] = offset_x;   /* +0x00: offset X */
    data[1] = offset_y;   /* +0x04: offset Y */
    data[2] = width;      /* +0x08: width */
    data[3] = height;     /* +0x0c: height */
    data[4] = damage;     /* +0x10: damage */
    data[5] = 1;          /* +0x14: active flag */
}

/* Hitbox_Update @ 0x0206f8c0 (288 bytes)
 * Updates hitbox position based on entity.
 * Args: r0=hitbox_data, r1=entity */
void Hitbox_Update(void *hitbox_data, void *entity) {
    s32 *hbox = (s32*)hitbox_data;
    s32 *ent = (s32*)entity;
    
    /* Calculate world position */
    s32 world_x = ent[2] + hbox[0];
    s32 world_y = ent[3] + hbox[1];
    
    /* Store calculated position */
    hbox[6] = world_x;   /* +0x18: world X */
    hbox[7] = world_y;   /* +0x1c: world Y */
}

/* Hitbox_Check @ 0x0206f9e0 (312 bytes)
 * Checks if hitbox collides with hurtbox.
 * Args: r0=hitbox_data, r1=hurtbox_data
 * Returns: 1 if colliding, 0 otherwise */
s32 Hitbox_Check(void *hitbox_data, void *hurtbox_data) {
    s32 *hbox = (s32*)hitbox_data;
    s32 *hurt = (s32*)hurtbox_data;
    
    /* Check if both are active */
    if (!hbox[5] || !hurt[4]) {
        return 0;
    }
    
    /* Get world positions and sizes */
    s32 hbox_x = hbox[6];
    s32 hbox_y = hbox[7];
    s32 hbox_w = hbox[2];
    s32 hbox_h = hbox[3];
    
    s32 hurt_x = hurt[5];
    s32 hurt_y = hurt[6];
    s32 hurt_w = hurt[2];
    s32 hurt_h = hurt[3];
    
    /* Test AABB intersection */
    if (hbox_x < hurt_x + hurt_w && hbox_x + hbox_w > hurt_x &&
        hbox_y < hurt_y + hurt_h && hbox_y + hbox_h > hurt_y) {
        return 1;
    }
    
    return 0;
}

/* Hitbox_GetDamage @ 0x0206fb18
 * Returns hitbox damage value.
 * Args: r0=hitbox_data
 * Returns: damage value */
u32 Hitbox_GetDamage(void *hitbox_data) {
    s32 *data = (s32*)hitbox_data;
    return data[4];
}

/* Hitbox_SetActive @ 0x0206fb40
 * Sets hitbox active state.
 * Args: r0=hitbox_data, r1=active */
void Hitbox_SetActive(void *hitbox_data, u32 active) {
    s32 *data = (s32*)hitbox_data;
    data[5] = active;
}

/* Hitbox_IsActive @ 0x0206fb60
 * Returns hitbox active state.
 * Args: r0=hitbox_data
 * Returns: 1 if active, 0 otherwise */
s32 Hitbox_IsActive(void *hitbox_data) {
    s32 *data = (s32*)hitbox_data;
    return data[5];
}
