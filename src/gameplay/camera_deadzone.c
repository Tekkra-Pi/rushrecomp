/* Camera deadzone functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_camdead_active;
static s32 s_camdead_x;
static s32 s_camdead_y;
static s32 s_camdead_w;
static s32 s_camdead_h;

void CameraDeadzone_Init(void) {
    s_camdead_active = 0;
    s_camdead_x = 0;
    s_camdead_y = 0;
    s_camdead_w = 0x800;
    s_camdead_h = 0x600;
}

void CameraDeadzone_Set(u32 index, u32 value) {
    (void)index;
    s_camdead_active = 1;
    g_camfollow_deadzone_x = value;
}

s32 CameraDeadzone_Update(void) {
    if (!s_camdead_active) return 0;
    return 1;
}

void CameraDeadzone_Stop(void) {
    s_camdead_active = 0;
}

s32 CameraDeadzone_IsActive(u32 index) {
    (void)index;
    return s_camdead_active;
}

void CameraDeadzone_GetRect(void) {
}

void CameraDeadzone_Reset(void) {
    s_camdead_active = 0;
    s_camdead_x = 0;
    s_camdead_y = 0;
}
