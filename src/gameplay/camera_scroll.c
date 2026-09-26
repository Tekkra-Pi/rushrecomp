/* Camera scroll functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_camsroll_active;
static u32 s_camsroll_timer;
static u32 s_camsroll_type;

void CameraScroll_Init(void) {
    s_camsroll_active = 0;
    s_camsroll_timer = 0;
    s_camsroll_type = 0;
}

void CameraScroll_Start(void) {
    s_camsroll_active = 1;
    s_camsroll_timer = 0;
    g_camscroll_timer = 0;
}

void CameraScroll_Update(void) {
    if (!s_camsroll_active) return;
    s_camsroll_timer++;
    g_camscroll_timer = s_camsroll_timer;
}

void CameraScroll_Stop(void) {
    s_camsroll_active = 0;
}

s32 CameraScroll_IsActive(u32 index) {
    (void)index;
    return s_camsroll_active;
}

u32 CameraScroll_GetType(void) { return s_camsroll_type; }
u32 CameraScroll_GetTimer(void) { return s_camsroll_timer; }

void CameraScroll_SetSpeed(u32 index, u32 value) {
    (void)index;
    (void)value;
}

void CameraScroll_Reset(void) {
    s_camsroll_active = 0;
    s_camsroll_timer = 0;
    s_camsroll_type = 0;
    g_camscroll_timer = 0;
}
