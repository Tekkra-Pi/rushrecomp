/* Cam zoom functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

void CamZoom_Init(void) {
    g_camzoom_active = 0;
    g_camzoom_level = 256;
    g_zoomcam_speed = 0;
    g_zoomcam_target = 256;
    g_zoomcam_zoom = 256;
}

void CamZoom_SetTarget(u32 index, u32 value) {
    (void)index;
    g_zoomcam_target = value;
    g_camzoom_active = 1;
}

void CamZoom_Update(void) {
    if (!g_camzoom_active) return;
    if (g_zoomcam_zoom < g_zoomcam_target) {
        g_zoomcam_zoom += g_zoomcam_speed;
        if (g_zoomcam_zoom > g_zoomcam_target) g_zoomcam_zoom = g_zoomcam_target;
    } else if (g_zoomcam_zoom > g_zoomcam_target) {
        if (g_zoomcam_zoom < g_zoomcam_speed) g_zoomcam_zoom = g_zoomcam_target;
        else g_zoomcam_zoom -= g_zoomcam_speed;
    }
    if (g_zoomcam_zoom == g_zoomcam_target) {
        g_camzoom_active = 0;
    }
}

void CamZoom_SetImmediate(u32 index, u32 value) {
    (void)index;
    g_zoomcam_zoom = value;
    g_zoomcam_target = value;
}

u32 CamZoom_GetLevel(void) { return g_zoomcam_zoom; }

s32 CamZoom_IsActive(u32 index) {
    (void)index;
    return g_camzoom_active;
}

void CamZoom_Reset(void) {
    g_camzoom_active = 0;
    g_camzoom_level = 256;
    g_zoomcam_speed = 0;
    g_zoomcam_target = 256;
    g_zoomcam_zoom = 256;
}
