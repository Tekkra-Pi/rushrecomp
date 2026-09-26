/* Gameplay zoom camera functions. */
#include "nds_types.h"

void ZoomCamera_Init(void) { g_camzoom_active = 0; g_camzoom_level = 0; g_zoomcam_speed = 0; g_zoomcam_target = 0; g_zoomcam_zoom = 0; }

void ZoomCamera_SetTarget(u32 index, u32 value) {
    (void)index;
    g_zoomcam_target = value;
    g_camzoom_active = 1;
}

void ZoomCamera_Update(void) {
    if (!g_camzoom_active) return;
    if (g_zoomcam_zoom < g_zoomcam_target) {
        g_zoomcam_zoom += g_zoomcam_speed;
        if (g_zoomcam_zoom > g_zoomcam_target) g_zoomcam_zoom = g_zoomcam_target;
    } else if (g_zoomcam_zoom > g_zoomcam_target) {
        if (g_zoomcam_zoom < g_zoomcam_speed) g_zoomcam_zoom = g_zoomcam_target;
        else g_zoomcam_zoom -= g_zoomcam_speed;
        if (g_zoomcam_zoom < g_zoomcam_target) g_zoomcam_zoom = g_zoomcam_target;
    }
    g_camzoom_level = g_zoomcam_zoom;
}

void ZoomCamera_Stop(void) { g_camzoom_active = 0; }
s32 ZoomCamera_IsActive(u32 index) { (void)index; return g_camzoom_active; }
u32 ZoomCamera_GetZoom(void) { return g_zoomcam_zoom; }
u32 ZoomCamera_GetTarget(void) { return g_zoomcam_target; }
void ZoomCamera_SetZoom(u32 index, u32 value) { (void)index; g_zoomcam_zoom = value; }
u32 ZoomCamera_GetProgress(void) { return g_zoomcam_zoom; }
void ZoomCamera_Reset(void) { g_camzoom_active = 0; g_camzoom_level = 0; g_zoomcam_speed = 0; g_zoomcam_target = 0; g_zoomcam_zoom = 0; }
