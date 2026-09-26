/* Collision shape functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* CollisionShape_Init @ 0x0200ec00 (148 bytes)
 * Initializes collision shape.
 * Args: r0=shape, r1=type */
void CollisionShape_Init(void *shape, u32 type) {
    u32 *data = (u32*)shape;
    data[0] = type;  /* Shape type */
    data[1] = 0;     /* Flags */
    data[2] = 0;     /* Layer mask */
}

/* CollisionShape_SetAABB @ 0x0200ec94 (216 bytes)
 * Sets AABB collision shape.
 * Args: r0=shape, r1=min_x, r2=min_y, r3=max_x, sp=max_y */
void CollisionShape_SetAABB(void *shape, s32 min_x, s32 min_y, s32 max_x, s32 max_y) {
    s32 *data = (s32*)shape;
    data[0] = 0x00;     /* AABB type */
    data[1] = min_x;    /* Min X */
    data[2] = min_y;    /* Min Y */
    data[3] = max_x;    /* Max X */
    data[4] = max_y;    /* Max Y */
}

/* CollisionShape_SetCircle @ 0x0200ed70 (264 bytes)
 * Sets circle collision shape.
 * Args: r0=shape, r1=x, r2=y, r3=radius */
void CollisionShape_SetCircle(void *shape, s32 x, s32 y, s32 radius) {
    s32 *data = (s32*)shape;
    data[0] = 0x01;     /* Circle type */
    data[1] = x;        /* Center X */
    data[2] = y;        /* Center Y */
    data[3] = radius;   /* Radius */
}

/* CollisionShape_SetPolygon @ 0x0200ee78
 * Sets polygon collision shape.
 * Args: r0=shape, r1=vertices, r2=count */
void CollisionShape_SetPolygon(void *shape, void *vertices, u32 count) {
    u32 *data = (u32*)shape;
    data[0] = 0x02;     /* Polygon type */
    data[1] = (u32)vertices;  /* Vertices pointer */
    data[2] = count;    /* Vertex count */
}

/* CollisionShape_GetType @ 0x0200eec0
 * Returns collision shape type.
 * Args: r0=shape
 * Returns: shape type */
u32 CollisionShape_GetType(void *shape) {
    u32 *data = (u32*)shape;
    return data[0];
}

/* CollisionShape_GetBounds @ 0x0200eee0
 * Returns bounding box of shape.
 * Args: r0=shape, r1=bounds */
void CollisionShape_GetBounds(void *shape, void *bounds) {
    u32 *data = (u32*)shape;
    s32 *out = (s32*)bounds;
    
    switch (data[0]) {
        case 0x00:  /* AABB */
            out[0] = data[1];
            out[1] = data[2];
            out[2] = data[3];
            out[3] = data[4];
            break;
            
        case 0x01:  /* Circle */
            out[0] = data[1] - data[3];
            out[1] = data[2] - data[3];
            out[2] = data[1] + data[3];
            out[3] = data[2] + data[3];
            break;
            
        default:
            out[0] = 0;
            out[1] = 0;
            out[2] = 0;
            out[3] = 0;
            break;
    }
}

/* CollisionShape_ContainsPoint @ 0x0200ef60
 * Checks if point is inside shape.
 * Args: r0=shape, r1=x, r2=y
 * Returns: 1 if contained, 0 otherwise */
s32 CollisionShape_ContainsPoint(void *shape, s32 x, s32 y) {
    u32 *data = (u32*)shape;
    
    switch (data[0]) {
        case 0x00:  /* AABB */
            return (x >= data[1] && x <= data[3] &&
                    y >= data[2] && y <= data[4]);
                    
        case 0x01:  /* Circle */
            s32 dx = x - data[1];
            s32 dy = y - data[2];
            return (dx * dx + dy * dy) <= (data[3] * data[3]);
            
        default:
            return 0;
    }
}

/* CollisionShape_SetLayer @ 0x0200f000
 * Sets collision layer mask.
 * Args: r0=shape, r1=layer */
void CollisionShape_SetLayer(void *shape, u32 layer) {
    u32 *data = (u32*)shape;
    data[2] = layer;
}

/* CollisionShape_GetLayer @ 0x0200f020
 * Returns collision layer mask.
 * Args: r0=shape
 * Returns: layer mask */
u32 CollisionShape_GetLayer(void *shape) {
    u32 *data = (u32*)shape;
    return data[2];
}
