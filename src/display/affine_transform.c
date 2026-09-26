/* Affine transform functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"


/* AffineTransform_Init @ 0x02006200 (192 bytes)
 * Initializes affine transform.
 * Args: r0=transform_data */
void AffineTransform_Init(void *transform_data) {
    s32 *data = (s32*)transform_data;
    
    /* Set identity matrix */
    data[0] = 0x100;   /* +0x00: PA (scale X) */
    data[1] = 0;       /* +0x04: PB (shear X) */
    data[2] = 0;       /* +0x08: PC (shear Y) */
    data[3] = 0x100;   /* +0x0c: PD (scale Y) */
    data[4] = 0;       /* +0x10: X offset */
    data[5] = 0;       /* +0x14: Y offset */
}

/* AffineTransform_SetRotation @ 0x020062c0 (288 bytes)
 * Sets rotation in affine transform.
 * Args: r0=transform_data, r1=angle */
void AffineTransform_SetRotation(void *transform_data, s32 angle) {
    s32 *data = (s32*)transform_data;
    
    /* Calculate sin/cos */
    s32 sin_val, cos_val;
    Math_SinCos(angle, &sin_val, &cos_val);
    
    /* Get current scale */
    s32 scale_x = data[0];
    s32 scale_y = data[3];
    
    /* Apply rotation with scale */
    data[0] = (cos_val * scale_x) >> 8;   /* PA */
    data[1] = (-sin_val * scale_x) >> 8;  /* PB */
    data[2] = (sin_val * scale_y) >> 8;   /* PC */
    data[3] = (cos_val * scale_y) >> 8;   /* PD */
}

/* AffineTransform_SetScale @ 0x020063e0 (312 bytes)
 * Sets scale in affine transform.
 * Args: r0=transform_data, r1=scale_x, r2=scale_y */
void AffineTransform_SetScale(void *transform_data, s32 scale_x, s32 scale_y) {
    s32 *data = (s32*)transform_data;
    
    /* Get current rotation */
    s32 pa = data[0];
    s32 pb = data[1];
    s32 pc = data[2];
    s32 pd = data[3];
    
    /* Calculate current angle from matrix */
    s32 angle = Math_ArcTan2(pb, pa);
    s32 sin_val, cos_val;
    Math_SinCos(angle, &sin_val, &cos_val);
    
    /* Apply scale with rotation */
    data[0] = (cos_val * scale_x) >> 8;   /* PA */
    data[1] = (-sin_val * scale_x) >> 8;  /* PB */
    data[2] = (sin_val * scale_y) >> 8;   /* PC */
    data[3] = (cos_val * scale_y) >> 8;   /* PD */
}

/* AffineTransform_SetPosition @ 0x02006518
 * Sets position in affine transform.
 * Args: r0=transform_data, r1=x, r2=y */
void AffineTransform_SetPosition(void *transform_data, s32 x, s32 y) {
    s32 *data = (s32*)transform_data;
    data[4] = x;  /* +0x10: X offset */
    data[5] = y;  /* +0x14: Y offset */
}

/* AffineTransform_Apply @ 0x02006560
 * Applies affine transform to point.
 * Args: r0=transform_data, r1=x, r2=y
 * Returns: transformed coordinates */
u32 AffineTransform_Apply(void *transform_data, s32 x, s32 y) {
    s32 *data = (s32*)transform_data;
    
    /* Apply matrix transformation */
    s32 new_x = ((data[0] * x + data[1] * y) >> 8) + data[4];
    s32 new_y = ((data[2] * x + data[3] * y) >> 8) + data[5];
    
    return (new_x & 0xffff) | ((new_y & 0xffff) << 16);
}

/* AffineTransform_Multiply @ 0x020065c0
 * Multiplies two affine transforms.
 * Args: r0=dest, r1=a, r2=b */
void AffineTransform_Multiply(void *dest, void *a, void *b) {
    s32 *d = (s32*)dest;
    s32 *aa = (s32*)a;
    s32 *bb = (s32*)b;
    
    /* Matrix multiplication */
    d[0] = ((aa[0] * bb[0] + aa[1] * bb[2]) >> 8);
    d[1] = ((aa[0] * bb[1] + aa[1] * bb[3]) >> 8);
    d[2] = ((aa[2] * bb[0] + aa[3] * bb[2]) >> 8);
    d[3] = ((aa[2] * bb[1] + aa[3] * bb[3]) >> 8);
    d[4] = ((aa[0] * bb[4] + aa[1] * bb[5] + aa[4]) >> 8);
    d[5] = ((aa[2] * bb[4] + aa[3] * bb[5] + aa[5]) >> 8);
}

/* AffineTransform_GetMatrix @ 0x02006600
 * Returns pointer to matrix data.
 * Args: r0=transform_data
 * Returns: matrix data pointer */
s32* AffineTransform_GetMatrix(void *transform_data) {
    return (s32*)transform_data;
}
