/* Camera shake functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static void CameraShake_StopInternal(void);
static u32 s_camshake_active;
static u32 s_camshake_timer;
static u32 s_camshake_intensity;

void CameraShake_Init(void) {
    s_camshake_active = 0;
    s_camshake_timer = 0;
    s_camshake_intensity = 0;
    g_camshake_active = 0;
}

void CameraShake_Start(void) {
    s_camshake_active = 1;
    s_camshake_timer = 0;
    s_camshake_intensity = 4;
    g_camshake_active = 1;
}

void CameraShake_Update(void) {
    if (!s_camshake_active) return;
    s_camshake_timer++;
    if (s_camshake_timer >= 20) {
        CameraShake_StopInternal();
        return;
    }
    if (s_camshake_timer & 1) {
        g_camera_x += s_camshake_intensity;
    } else {
        g_camera_x -= s_camshake_intensity;
    }
    if (s_camshake_timer % 4 == 0 && s_camshake_intensity > 1) {
        s_camshake_intensity--;
    }
}

static void CameraShake_StopInternal(void) {
    s_camshake_active = 0;
    g_camshake_active = 0;
}

void CameraShake_Stop(void) {
    CameraShake_StopInternal();
}

s32 CameraShake_IsActive(u32 index) {
    (void)index;
    return s_camshake_active;
}

void CameraShake_Reset(void) {
    s_camshake_active = 0;
    s_camshake_timer = 0;
    s_camshake_intensity = 0;
    g_camshake_active = 0;
}
