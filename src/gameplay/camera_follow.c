/* Camera follow functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern void Player_GetPosition(s32 *x, s32 *y);

void CameraFollow_Init(void) {
    g_camfollow_deadzone_x = 0x400;
    g_camfollow_deadzone_y = 0x300;
    g_camfollow_speed_x = 0x100;
    g_camfollow_speed_y = 0x100;
}

void CameraFollow_Update(void) {
    s32 px, py;
    Player_GetPosition(&px, &py);
    s32 target_x = px - g_window_width / 2;
    s32 target_y = py - g_window_height / 2;
    s32 dx = target_x - g_camera_x;
    s32 dy = target_y - g_camera_y;
    if (dx > (s32)g_camfollow_deadzone_x) g_camera_x += g_camfollow_speed_x;
    else if (dx < -(s32)g_camfollow_deadzone_x) g_camera_x -= g_camfollow_speed_x;
    if (dy > (s32)g_camfollow_deadzone_y) g_camera_y += g_camfollow_speed_y;
    else if (dy < -(s32)g_camfollow_deadzone_y) g_camera_y -= g_camfollow_speed_y;
    g_camera_target_x = target_x;
    g_camera_target_y = target_y;
}

void CameraFollow_Reset(void) {
    g_camfollow_deadzone_x = 0x400;
    g_camfollow_deadzone_y = 0x300;
    g_camfollow_speed_x = 0x100;
    g_camfollow_speed_y = 0x100;
}
