/* Camera lerp functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_camlrp_active;
static u32 s_camlrp_timer;

void CameraLerp_Init(void) {
    s_camlrp_active = 0;
    s_camlrp_timer = 0;
}

void CameraLerp_Start(void) {
    s_camlrp_active = 1;
    s_camlrp_timer = 0;
}

void CameraLerp_Update(void) {
    if (!s_camlrp_active) return;
    s_camlrp_timer++;
    g_camera_x += (g_camera_target_x - g_camera_x) / 8;
    g_camera_y += (g_camera_target_y - g_camera_y) / 8;
}

void CameraLerp_Skip(void) {
    g_camera_x = g_camera_target_x;
    g_camera_y = g_camera_target_y;
    s_camlrp_active = 0;
}

s32 CameraLerp_IsActive(u32 index) {
    (void)index;
    return s_camlrp_active;
}

void CameraLerp_Reset(void) {
    s_camlrp_active = 0;
    s_camlrp_timer = 0;
}
