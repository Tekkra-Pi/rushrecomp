/* Gameplay level bounds functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* LevelBounds_Init @ 0x0202e400 (168 bytes)
 * Initializes level bounds. */
void LevelBounds_Init(void) {
    g_levelbounds_left = 0;
    g_levelbounds_top = 0;
    g_levelbounds_right = 0x100000;
    g_levelbounds_bottom = 0x100000;
}

/* LevelBounds_Set @ 0x0202e4a8 (232 bytes)
 * Sets level bounds.
 * Args: r0=left, r1=top, r2=right, r3=bottom */
void LevelBounds_Set(s32 left, s32 top, s32 right, s32 bottom) {
    g_levelbounds_left = left;
    g_levelbounds_top = top;
    g_levelbounds_right = right;
    g_levelbounds_bottom = bottom;
}

/* LevelBounds_Check @ 0x0202e590 (296 bytes)
 * Checks if position is within bounds.
 * Args: r0=x, r1=y
 * Returns: 1 if within bounds, 0 otherwise */
s32 LevelBounds_Check(s32 x, s32 y) {
    return x >= g_levelbounds_left && x <= g_levelbounds_right &&
           y >= g_levelbounds_top && y <= g_levelbounds_bottom;
}

/* LevelBounds_Clamp @ 0x0202e6b8
 * Clamps position to bounds.
 * Args: r0=in_x, r1=in_y, r2=out_x, r3=out_y */
void LevelBounds_Clamp(s32 in_x, s32 in_y, s32 *out_x, s32 *out_y) {
    *out_x = in_x;
    *out_y = in_y;
    if (*out_x < g_levelbounds_left) *out_x = g_levelbounds_left;
    if (*out_x > g_levelbounds_right) *out_x = g_levelbounds_right;
    if (*out_y < g_levelbounds_top) *out_y = g_levelbounds_top;
    if (*out_y > g_levelbounds_bottom) *out_y = g_levelbounds_bottom;
}

/* LevelBounds_GetLeft @ 0x0202e760
 * Returns left bound.
 * Returns: left */
s32 LevelBounds_GetLeft(void) {
    return g_levelbounds_left;
}

/* LevelBounds_GetTop @ 0x0202e780
 * Returns top bound.
 * Returns: top */
s32 LevelBounds_GetTop(void) {
    return g_levelbounds_top;
}

/* LevelBounds_GetRight @ 0x0202e7a0
 * Returns right bound.
 * Returns: right */
s32 LevelBounds_GetRight(void) {
    return g_levelbounds_right;
}

/* LevelBounds_GetBottom @ 0x0202e7c0
 * Returns bottom bound.
 * Returns: bottom */
s32 LevelBounds_GetBottom(void) {
    return g_levelbounds_bottom;
}

/* LevelBounds_GetWidth @ 0x0202e7e0
 * Returns level width.
 * Returns: width */
s32 LevelBounds_GetWidth(void) {
    return g_levelbounds_right - g_levelbounds_left;
}

/* LevelBounds_GetHeight @ 0x0202e800
 * Returns level height.
 * Returns: height */
s32 LevelBounds_GetHeight(void) {
    return g_levelbounds_bottom - g_levelbounds_top;
}

/* LevelBounds_Reset @ 0x0202e820
 * Resets level bounds. */
void LevelBounds_Reset(void) {
    g_levelbounds_left = 0;
    g_levelbounds_top = 0;
    g_levelbounds_right = 0x100000;
    g_levelbounds_bottom = 0x100000;
}
