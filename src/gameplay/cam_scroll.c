/* Cam scroll functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static s32 s_camscroll_target_x;
static s32 s_camscroll_target_y;
static s32 s_camscroll_pos_x;
static s32 s_camscroll_pos_y;

void CamScroll_Init(void) {
    s_camscroll_target_x = 0;
    s_camscroll_target_y = 0;
    s_camscroll_pos_x = 0;
    s_camscroll_pos_y = 0;
    g_canscroll_target_x = 0;
    g_canscroll_target_y = 0;
}

void CamScroll_SetTarget(u32 index, u32 value) {
    (void)index;
    s_camscroll_target_x = (s32)value;
    g_canscroll_target_x = value;
}

void CamScroll_GetTarget(void) { }

void CamScroll_Update(void) {
    s_camscroll_pos_x += (s_camscroll_target_x - s_camscroll_pos_x) / 8;
    s_camscroll_pos_y += (s_camscroll_target_y - s_camscroll_pos_y) / 8;
    g_camera_x = s_camscroll_pos_x;
    g_camera_y = s_camscroll_pos_y;
}

void CamScroll_SetPosition(u32 index, u32 value) {
    (void)index;
    s_camscroll_pos_x = (s32)value;
}

void CamScroll_GetPosition(void) { }

void CamScroll_Reset(void) {
    s_camscroll_target_x = 0;
    s_camscroll_target_y = 0;
    s_camscroll_pos_x = 0;
    s_camscroll_pos_y = 0;
}
