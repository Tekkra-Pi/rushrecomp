/* Gameplay level camera bounds functions. */

#include "nds_types.h"

#define CAMBOUNDS_MAX 16

void CameraBounds_Init(void) {
    g_cambounds_left = 0;
    g_cambounds_right = 0;
    g_cambounds_top = 0;
    g_cambounds_bottom = 0;
}

void CameraBounds_Set(s32 left, s32 right, s32 top, s32 bottom) {
    g_cambounds_left = left;
    g_cambounds_right = right;
    g_cambounds_top = top;
    g_cambounds_bottom = bottom;
}

void CameraBounds_Update(void) {
    if (g_camera_x < (s32)g_cambounds_left) g_camera_x = g_cambounds_left;
    if (g_camera_x > (s32)g_cambounds_right) g_camera_x = g_cambounds_right;
    if (g_camera_y < (s32)g_cambounds_top) g_camera_y = g_cambounds_top;
    if (g_camera_y > (s32)g_cambounds_bottom) g_camera_y = g_cambounds_bottom;
}

s32 CameraBounds_GetLeft(void) { return g_cambounds_left; }
s32 CameraBounds_GetRight(void) { return g_cambounds_right; }
s32 CameraBounds_GetTop(void) { return g_cambounds_top; }
s32 CameraBounds_GetBottom(void) { return g_cambounds_bottom; }
