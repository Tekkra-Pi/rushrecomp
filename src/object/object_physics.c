/* Object physics functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "object_list.h"


/* ObjectPhysics_Init @ 0x02012400 (168 bytes)
 * Initializes object physics.
 * Args: r0=object */
void ObjectPhysics_Init(void *object) {
    u32 *data = (u32*)object;
    
    /* Clear physics state */
    data[8] = 0;   /* +0x20: vel_x */
    data[9] = 0;   /* +0x24: vel_y */
    data[10] = 0;  /* +0x28: accel_x */
    data[11] = 0;  /* +0x2c: accel_y */
}

/* ObjectPhysics_Update @ 0x020124a8 (232 bytes)
 * Updates object physics.
 * Args: r0=object */
void ObjectPhysics_Update(void *object) {
    u32 *data = (u32*)object;
    
    /* Apply acceleration */
    data[8] += data[10];  /* vel_x += accel_x */
    data[9] += data[11];  /* vel_y += accel_y */
    
    /* Apply gravity */
    data[9] += 0x20;  /* gravity */
    
    /* Apply velocity */
    data[2] += data[8];  /* x += vel_x */
    data[3] += data[9];  /* y += vel_y */
}

/* ObjectPhysics_SetVelocity @ 0x02012590 (296 bytes)
 * Sets object velocity.
 * Args: r0=object, r1=vel_x, r2=vel_y */
void ObjectPhysics_SetVelocity(void *object, s32 vel_x, s32 vel_y) {
    u32 *data = (u32*)object;
    data[8] = vel_x;
    data[9] = vel_y;
}

/* ObjectPhysics_GetVelocity @ 0x020126b8
 * Returns object velocity.
 * Args: r0=object, r1=out */
void ObjectPhysics_GetVelocity(void *object, void *out) {
    u32 *data = (u32*)object;
    s32 *result = (s32*)out;
    result[0] = data[8];
    result[1] = data[9];
}

/* ObjectPhysics_SetAcceleration @ 0x02012700
 * Sets object acceleration.
 * Args: r0=object, r1=accel_x, r2=accel_y */
void ObjectPhysics_SetAcceleration(void *object, s32 accel_x, s32 accel_y) {
    u32 *data = (u32*)object;
    data[10] = accel_x;
    data[11] = accel_y;
}

/* ObjectPhysics_Stop @ 0x02012740
 * Stops object movement.
 * Args: r0=object */
void ObjectPhysics_Stop(void *object) {
    u32 *data = (u32*)object;
    data[8] = 0;
    data[9] = 0;
    data[10] = 0;
    data[11] = 0;
}

/* ObjectPhysics_ApplyFriction @ 0x02012780
 * Applies friction to object.
 * Args: r0=object, r1=friction */
void ObjectPhysics_ApplyFriction(void *object, u32 friction) {
    u32 *data = (u32*)object;
    data[8] = (data[8] * friction) >> 8;
    data[9] = (data[9] * friction) >> 8;
}

/* ObjectPhysics_ApplyBounce @ 0x020127e0
 * Applies bounce to object.
 * Args: r0=object, r1=bounce */
void ObjectPhysics_ApplyBounce(void *object, u32 bounce) {
    u32 *data = (u32*)object;
    data[9] = -(data[9] * bounce) >> 8;
}

/* ObjectPhysics_IsMoving @ 0x02012840
 * Returns whether object is moving.
 * Args: r0=object
 * Returns: 1 if moving, 0 otherwise */
s32 ObjectPhysics_IsMoving(void *object) {
    u32 *data = (u32*)object;
    return (data[8] != 0 || data[9] != 0);
}

/* ObjectPhysics_GetSpeed @ 0x02012880
 * Returns object speed.
 * Args: r0=object
 * Returns: speed */
s32 ObjectPhysics_GetSpeed(void *object) {
    u32 *data = (u32*)object;
    return Math_Sqrt(data[8] * data[8] + data[9] * data[9]);
}
