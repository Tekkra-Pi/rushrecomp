/* Hurtbox and hitbox functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "player.h"

/* Hurtbox_Init @ 0x0206f000 (188 bytes)
 * Initializes a hurtbox.
 * Args: r0=hurtbox_data, r1=offset_x, r2=offset_y, r3=width, sp=height */
void Hurtbox_Init(void *hurtbox_data, s32 offset_x, s32 offset_y, s32 width, s32 height) {
    s32 *data = (s32*)hurtbox_data;
    
    /* Set hurtbox parameters */
    data[0] = offset_x;   /* +0x00: offset X */
    data[1] = offset_y;   /* +0x04: offset Y */
    data[2] = width;      /* +0x08: width */
    data[3] = height;     /* +0x0c: height */
    data[4] = 1;          /* +0x10: active flag */
}

/* Hurtbox_Update @ 0x0206f0bc (256 bytes)
 * Updates hurtbox position based on entity.
 * Args: r0=hurtbox_data, r1=entity */
void Hurtbox_Update(void *hurtbox_data, void *entity) {
    s32 *hbox = (s32*)hurtbox_data;
    s32 *ent = (s32*)entity;
    
    /* Calculate world position */
    s32 world_x = ent[2] + hbox[0];  /* entity_x + offset_x */
    s32 world_y = ent[3] + hbox[1];  /* entity_y + offset_y */
    
    /* Store calculated position */
    hbox[5] = world_x;   /* +0x14: world X */
    hbox[6] = world_y;   /* +0x18: world Y */
}

/* Hurtbox_Check @ 0x0206f1bc (296 bytes)
 * Checks collision between two hurtboxes.
 * Args: r0=hurtbox_a, r1=hurtbox_b
 * Returns: 1 if colliding, 0 otherwise */
s32 Hurtbox_Check(void *hurtbox_a, void *hurtbox_b) {
    s32 *a = (s32*)hurtbox_a;
    s32 *b = (s32*)hurtbox_b;
    
    /* Get world positions and sizes */
    s32 a_x = a[5];
    s32 a_y = a[6];
    s32 a_w = a[2];
    s32 a_h = a[3];
    
    s32 b_x = b[5];
    s32 b_y = b[6];
    s32 b_w = b[2];
    s32 b_h = b[3];
    
    /* Test AABB intersection */
    if (a_x < b_x + b_w && a_x + a_w > b_x &&
        a_y < b_y + b_h && a_y + a_h > b_y) {
        return 1;
    }
    
    return 0;
}

/* HitboxGroup_Init @ 0x0206f400 (168 bytes)
 * Initializes a hitbox group.
 * Args: r0=group_data, r1=num_hitboxes */
void HitboxGroup_Init(void *group_data, u32 num_hitboxes) {
    u32 *data = (u32*)group_data;
    
    /* Set group parameters */
    data[0] = num_hitboxes;  /* +0x00: number of hitboxes */
    data[1] = 0;             /* +0x04: active hitbox index */
    data[2] = 0;             /* +0x08: flags */
    
    /* Initialize hitbox array */
    /* ... */
}

/* HitboxGroup_Switch @ 0x0206f4a8 (232 bytes)
 * Switches active hitbox in group.
 * Args: r0=group_data, r1=new_index */
void HitboxGroup_Switch(void *group_data, u32 new_index) {
    u32 *data = (u32*)group_data;
    u32 num_hitboxes = data[0];
    
    /* Validate index */
    if (new_index >= num_hitboxes) {
        return;
    }
    
    /* Deactivate current hitbox */
    u32 current = data[1];
    /* hitboxes[current].active = 0; */
    
    /* Activate new hitbox */
    data[1] = new_index;
    /* hitboxes[new_index].active = 1; */
}

/* HitboxGroup_GetActive @ 0x0206f590 (296 bytes)
 * Returns active hitbox in group.
 * Args: r0=group_data
 * Returns: active hitbox data or NULL */
void* HitboxGroup_GetActive(void *group_data) {
    u32 *data = (u32*)group_data;
    u32 active_index = data[1];
    u32 num_hitboxes = data[0];
    
    if (active_index >= num_hitboxes) {
        return NULL;
    }
    
    /* Return pointer to active hitbox */
    return (void*)&data[4 + (active_index * 8)];
}

/* Hurtbox_SetActive @ 0x0206f6c8
 * Sets hurtbox active state.
 * Args: r0=hurtbox_data, r1=active */
void Hurtbox_SetActive(void *hurtbox_data, u32 active) {
    s32 *data = (s32*)hurtbox_data;
    data[4] = active;
}

/* Hurtbox_IsActive @ 0x0206f6f0
 * Returns hurtbox active state.
 * Args: r0=hurtbox_data
 * Returns: 1 if active, 0 otherwise */
s32 Hurtbox_IsActive(void *hurtbox_data) {
    s32 *data = (s32*)hurtbox_data;
    return data[4];
}
