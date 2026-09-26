/* Gameplay camera bounds functions. */
#include "nds_types.h"

void CameraBounds_Init(void) {
    g_cambounds_left = 0;
    g_cambounds_top = 0;
    g_cambounds_right = 256;
    g_cambounds_bottom = 192;
}

void CameraBounds_Set(s32 left, s32 top, s32 right, s32 bottom) {
    g_cambounds_left = left;
    g_cambounds_top = top;
    g_cambounds_right = right;
    g_cambounds_bottom = bottom;
}

void CameraBounds_Clamp(s32 *x, s32 *y) {
    if (*x < g_cambounds_left) *x = g_cambounds_left;
    if (*x > g_cambounds_right) *x = g_cambounds_right;
    if (*y < g_cambounds_top) *y = g_cambounds_top;
    if (*y > g_cambounds_bottom) *y = g_cambounds_bottom;
}

s32 CameraBounds_GetLeft(void) { return g_cambounds_left; }
s32 CameraBounds_GetTop(void) { return g_cambounds_top; }
s32 CameraBounds_GetRight(void) { return g_cambounds_right; }
s32 CameraBounds_GetBottom(void) { return g_cambounds_bottom; }
s32 CameraBounds_Check(s32 x, s32 y) {
    return x >= g_cambounds_left && x <= g_cambounds_right &&
           y >= g_cambounds_top && y <= g_cambounds_bottom;
}
void CameraBounds_Reset(void) { g_cambounds_left = 0; g_cambounds_top = 0; g_cambounds_right = 256; g_cambounds_bottom = 192; }
