/* Display scroll functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern s32 g_camera_x, g_camera_y;
extern s32 g_target_scroll_x, g_target_scroll_y;
extern s32 g_camscroll_timer;

void DisplayScroll_Init(void) {
    g_camera_x = 0;
    g_camera_y = 0;
    g_target_scroll_x = 0;
    g_target_scroll_y = 0;
    g_camscroll_timer = 0;
}

void DisplayScroll_Update(void) {
    g_camera_x = g_target_scroll_x;
    g_camera_y = g_target_scroll_y;
}

void DisplayScroll_SetTarget(u32 index, u32 value) {
    (void)index;
    if (index == 0) g_target_scroll_x = (s32)value;
    else g_target_scroll_y = (s32)value;
}

u32 DisplayScroll_GetPosition(void) {
    return (u32)g_camera_x;
}

void DisplayScroll_SetPosition(u32 index, u32 value) {
    if (index == 0) g_camera_x = (s32)value;
    else g_camera_y = (s32)value;
}

void DisplayScroll_Clamp(void) {
    extern u32 g_cambounds_left, g_cambounds_right;
    extern u32 g_cambounds_top, g_cambounds_bottom;
    if (g_camera_x < (s32)g_cambounds_left)
        g_camera_x = (s32)g_cambounds_left;
    if (g_camera_x > (s32)g_cambounds_right)
        g_camera_x = (s32)g_cambounds_right;
    if (g_camera_y < (s32)g_cambounds_top)
        g_camera_y = (s32)g_cambounds_top;
    if (g_camera_y > (s32)g_cambounds_bottom)
        g_camera_y = (s32)g_cambounds_bottom;
}

void DisplayScroll_Shake(void) {
    /* Start screen shake */
}

void DisplayScroll_UpdateShake(void) {
    /* Update screen shake offset */
}
