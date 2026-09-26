/* Collision point functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"


/* CollisionPoint_Init @ 0x0200f400 (192 bytes)
 * Initializes collision point.
 * Args: r0=point, r1=x, r2=y */
void CollisionPoint_Init(void *point, s32 x, s32 y) {
    s32 *data = (s32*)point;
    data[0] = x;    /* X coordinate */
    data[1] = y;    /* Y coordinate */
    data[2] = 0;    /* Normal X */
    data[3] = 0;    /* Normal Y */
    data[4] = 0;    /* Flags */
}

/* CollisionPoint_Set @ 0x0200f4c0 (288 bytes)
 * Sets collision point position.
 * Args: r0=point, r1=x, r2=y */
void CollisionPoint_Set(void *point, s32 x, s32 y) {
    s32 *data = (s32*)point;
    data[0] = x;
    data[1] = y;
}

/* CollisionPoint_Get @ 0x0200f5e0 (312 bytes)
 * Returns collision point position.
 * Args: r0=point, r1=out */
void CollisionPoint_Get(void *point, void *out) {
    s32 *data = (s32*)point;
    s32 *result = (s32*)out;
    result[0] = data[0];
    result[1] = data[1];
}

/* CollisionPoint_SetNormal @ 0x0200f6f8
 * Sets collision point normal.
 * Args: r0=point, r1=nx, r2=ny */
void CollisionPoint_SetNormal(void *point, s32 nx, s32 ny) {
    s32 *data = (s32*)point;
    data[2] = nx;
    data[3] = ny;
}

/* CollisionPoint_GetNormal @ 0x0200f740
 * Returns collision point normal.
 * Args: r0=point, r1=out */
void CollisionPoint_GetNormal(void *point, void *out) {
    s32 *data = (s32*)point;
    s32 *result = (s32*)out;
    result[0] = data[2];
    result[1] = data[3];
}

/* CollisionPoint_GetDistance @ 0x0200f780
 * Returns distance from point to origin.
 * Args: r0=point
 * Returns: distance */
s32 CollisionPoint_GetDistance(void *point) {
    s32 *data = (s32*)point;
    return Math_Sqrt(data[0] * data[0] + data[1] * data[1]);
}

/* CollisionPoint_Translate @ 0x0200f7c0
 * Translates collision point.
 * Args: r0=point, r1=dx, r2=dy */
void CollisionPoint_Translate(void *point, s32 dx, s32 dy) {
    s32 *data = (s32*)point;
    data[0] += dx;
    data[1] += dy;
}

/* CollisionPoint_Rotate @ 0x0200f800
 * Rotates collision point around origin.
 * Args: r0=point, r1=angle */
void CollisionPoint_Rotate(void *point, s32 angle) {
    s32 *data = (s32*)point;
    s32 sin_val, cos_val;
    Math_SinCos(angle, &sin_val, &cos_val);
    
    s32 new_x = (data[0] * cos_val - data[1] * sin_val) >> 8;
    s32 new_y = (data[0] * sin_val + data[1] * cos_val) >> 8;
    
    data[0] = new_x;
    data[1] = new_y;
}

/* CollisionPoint_Dot @ 0x0200f8a8
 * Returns dot product of two collision points.
 * Args: r0=a, r1=b
 * Returns: dot product */
s32 CollisionPoint_Dot(void *a, void *b) {
    s32 *aa = (s32*)a;
    s32 *bb = (s32*)b;
    return (aa[0] * bb[0] + aa[1] * bb[1]) >> 8;
}

/* CollisionPoint_Cross @ 0x0200f8f0
 * Returns cross product of two collision points.
 * Args: r0=a, r1=b
 * Returns: cross product */
s32 CollisionPoint_Cross(void *a, void *b) {
    s32 *aa = (s32*)a;
    s32 *bb = (s32*)b;
    return (aa[0] * bb[1] - aa[1] * bb[0]) >> 8;
}
