/* Cam pan functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

void CamPan_Init(void) {
    g_campan_active = 0;
    g_campan_end_x = 0;
    g_campan_end_y = 0;
}

void CamPan_Start(void) {
    g_campan_active = 1;
}

void CamPan_Update(void) {
    if (!g_campan_active) return;
    s32 dx = g_campan_end_x - g_camera_x;
    s32 dy = g_campan_end_y - g_camera_y;
    if (dx > 0) g_camera_x += 0x100;
    else if (dx < 0) g_camera_x -= 0x100;
    if (dy > 0) g_camera_y += 0x100;
    else if (dy < 0) g_camera_y -= 0x100;
}

s32 CamPan_IsActive(u32 index) {
    (void)index;
    return g_campan_active;
}

void CamPan_Reset(void) {
    g_campan_active = 0;
    g_campan_end_x = 0;
    g_campan_end_y = 0;
}
