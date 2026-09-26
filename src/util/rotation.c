/* Rotation and scale functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 *
 * Manages rotation angles and scale values for affine-transformed sprites.
 * Uses lookup tables for sin/cos computation.
 */

#include "nds_types.h"

/* Forward declarations for math functions */
extern s32 Math_Sin(u32 angle);
extern s32 Math_Cos(u32 angle);

/* Rotation/scale state */
typedef struct {
    s16 angle;          /* +0x00: current rotation angle (8-bit fixed point) */
    s16 _pad;           /* +0x02: padding */
    s32 sin_val;        /* +0x04: cached sin value */
    s32 cos_val;        /* +0x08: cached cos value */
    s16 scale_x;        /* +0x0c: horizontal scale (8.8 fixed) */
    s16 scale_y;        /* +0x0e: vertical scale (8.8 fixed) */
} AffineState;

extern AffineState g_affine_state;
extern s32 g_sin_table[];
extern s32 g_cos_table[];

/* ======================================================================== */
/* Rotation_Set                                                              */
/* Sets the rotation angle and pre-computes sin/cos.                        */
/* Args: r0=index (unused, single global), r1=value (angle)                 */
/* ======================================================================== */
void Rotation_Set(u32 index, u32 value) {
    (void)index;
    g_affine_state.angle = (s16)value;
    g_affine_state.sin_val = Math_Sin(value);
    g_affine_state.cos_val = Math_Cos(value);
}

/* ======================================================================== */
/* Rotation_Get                                                              */
/* Returns the current rotation angle.                                      */
/* ======================================================================== */
s32 Rotation_Get(void) {
    return g_affine_state.angle;
}

/* ======================================================================== */
/* Rotation_Apply                                                            */
/* Applies the current rotation to compute affine matrix parameters.       */
/* Returns a packed value suitable for OAM affine parameter writes.         */
/* ======================================================================== */
u32 Rotation_Apply(void) {
    s32 sin = g_affine_state.sin_val;
    s32 cos = g_affine_state.cos_val;

    /* Pack sin/cos into OAM-compatible format */
    /* OAM affine params are 1.8.7 fixed-point halfwords */
    s16 param_a = (s16)(cos >> 4);
    s16 param_b = (s16)(sin >> 4);

    return ((u32)(u16)param_a) | (((u32)(u16)param_b) << 16);
}

/* ======================================================================== */
/* Scale_Set                                                                 */
/* Sets the horizontal and vertical scale values.                           */
/* Args: r0=index (unused), r1=value (packed scale)                         */
/* ======================================================================== */
void Scale_Set(u32 index, u32 value) {
    (void)index;
    g_affine_state.scale_x = (s16)(value & 0xFFFF);
    g_affine_state.scale_y = (s16)((value >> 16) & 0xFFFF);
}

/* ======================================================================== */
/* Scale_Get                                                                 */
/* Returns the current scale value (packed x|y).                            */
/* ======================================================================== */
s32 Scale_Get(void) {
    return ((s32)(u16)g_affine_state.scale_x) |
           (((s32)(u16)g_affine_state.scale_y) << 16);
}
