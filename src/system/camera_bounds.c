/* Camera bounds and level boundary functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern s32 g_camera_x, g_camera_y;
extern s32 g_camera_target_x, g_camera_target_y;
extern u32 g_cambounds_left, g_cambounds_right;
extern u32 g_cambounds_top, g_cambounds_bottom;
extern s32 g_camdead_w, g_camdead_h;
extern u32 g_camfollow_deadzone_x, g_camfollow_deadzone_y;
extern u32 g_camfollow_speed_x, g_camfollow_speed_y;

void Camera_ClampPlayer(void) {
    s32 half_w = (s32)g_camdead_w;
    s32 half_h = (s32)g_camdead_h;

    if (g_camera_target_x < (s32)g_cambounds_left + half_w)
        g_camera_target_x = (s32)g_cambounds_left + half_w;
    if (g_camera_target_x > (s32)g_cambounds_right - half_w)
        g_camera_target_x = (s32)g_cambounds_right - half_w;
    if (g_camera_target_y < (s32)g_cambounds_top + half_h)
        g_camera_target_y = (s32)g_cambounds_top + half_h;
    if (g_camera_target_y > (s32)g_cambounds_bottom - half_h)
        g_camera_target_y = (s32)g_cambounds_bottom - half_h;
}

void Camera_UpdateBounds(void) {
    extern u32 g_levelbounds_left, g_levelbounds_right;
    extern u32 g_levelbounds_top, g_levelbounds_bottom;
    g_cambounds_left = g_levelbounds_left;
    g_cambounds_right = g_levelbounds_right;
    g_cambounds_top = g_levelbounds_top;
    g_cambounds_bottom = g_levelbounds_bottom;
    Camera_ClampPlayer();
}
