/* Math/trig functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern const s16 g_sin_table[256];

s32 Math_ArcTan(s32 x, s32 y);

/* Math_SinCos @ 0x02009ab4 (260 bytes)
 * Calculates sine and cosine of an angle.
 * Args: r0=angle (0-255 for 0-360 degrees)
 * Output: stores sin/cos to provided pointers */
void Math_SinCos(u32 angle, s32 *sin_out, s32 *cos_out) {
    u32 quadrant;
    u32 index;
    s32 sin_val, cos_val;
    
    /* Normalize angle to 0-255 */
    angle &= 0xff;
    
    /* Determine quadrant */
    quadrant = angle >> 6;  /* 0-3 */
    index = angle & 0x3f;   /* 0-63 within quadrant */
    
    /* Look up sin/cos from table */
    /* Sin table at 0x2089b54 */
    sin_val = g_sin_table[index];
    
    /* Cos table is sin table offset by 64 */
    cos_val = g_sin_table[(index + 64) & 0xff];
    
    /* Apply quadrant sign */
    switch (quadrant) {
        case 0:  /* 0-90 */
            sin_val = sin_val;
            cos_val = cos_val;
            break;
        case 1:  /* 90-180 */
            sin_val = sin_val;
            cos_val = -cos_val;
            break;
        case 2:  /* 180-270 */
            sin_val = -sin_val;
            cos_val = -cos_val;
            break;
        case 3:  /* 270-360 */
            sin_val = -sin_val;
            cos_val = cos_val;
            break;
    }
    
    *sin_out = sin_val;
    *cos_out = cos_val;
}

/* Math_Sqrt @ 0x02009bc8 (216 bytes)
 * Integer square root.
 * Args: r0=value
 * Returns: square root */
u32 Math_Sqrt(u32 value) {
    u32 result = 0;
    u32 bit = 1 << 30;
    
    /* Handle zero */
    if (value == 0) return 0;
    
    /* Newton's method */
    while (bit > value) {
        bit >>= 2;
    }
    
    while (bit != 0) {
        if (value >= result + bit) {
            value -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }
    
    return result;
}

/* Math_ArcTan2 @ 0x02009ca8 (180 bytes)
 * Arctangent of y/x.
 * Args: r0=y, r1=x
 * Returns: angle (0-255 for 0-360 degrees) */
u32 Math_ArcTan2(s32 y, s32 x) {
    u32 angle;
    s32 abs_y, abs_x;
    
    /* Handle zero case */
    if (x == 0 && y == 0) {
        return 0;
    }
    
    /* Get absolute values */
    abs_y = (y < 0) ? -y : y;
    abs_x = (x < 0) ? -x : x;
    
    /* Determine quadrant and calculate angle */
    if (x >= 0) {
        if (y >= 0) {
            /* Quadrant 1 */
            angle = Math_ArcTan(abs_y, abs_x);
        } else {
            /* Quadrant 4 */
            angle = 256 - Math_ArcTan(abs_y, abs_x);
        }
    } else {
        if (y >= 0) {
            /* Quadrant 2 */
            angle = 128 - Math_ArcTan(abs_y, abs_x);
        } else {
            /* Quadrant 3 */
            angle = 128 + Math_ArcTan(abs_y, abs_x);
        }
    }
    
    return angle & 0xff;
}

/* Math_ArcTan - lookup-based arctangent */
s32 Math_ArcTan(s32 x, s32 y) {
    if (x == 0) return 0;
    /* Simple approximation using linear interpolation */
    u32 angle = 0;
    if (y > x) {
        angle = 64 - (x * 64 / y);
    } else if (x > 0) {
        angle = y * 64 / x;
    }
    return (s32)angle;
}

/* Math_Sin - returns sine * 256 for angle 0-255 */
s32 Math_Sin(s32 angle) {
    u32 idx = (u32)angle & 0xff;
    return (s32)g_sin_table[idx];
}

/* Math_Cos - returns cosine * 256 for angle 0-255 */
s32 Math_Cos(s32 angle) {
    u32 idx = ((u32)angle + 64) & 0xff;
    return (s32)g_sin_table[idx];
}

/* Math_Rand - simple PRNG */
static u32 g_math_rand_state = 0x12345678;
u32 Math_Rand(void) {
    g_math_rand_state = g_math_rand_state * 1103515245 + 12345;
    return (g_math_rand_state >> 16) & 0x7fff;
}
