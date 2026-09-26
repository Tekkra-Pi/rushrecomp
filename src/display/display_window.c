/* Display window functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_dispwin_active;
extern u32 g_window_layer_mask;
extern u32 g_window_width, g_window_height;
extern u32 g_window_x, g_window_y;

void DisplayWindow_Init(void) {
    g_dispwin_active = 0;
    g_window_layer_mask = 0;
    g_window_width = 0;
    g_window_height = 0;
    g_window_x = 0;
    g_window_y = 0;
    REG_WININ = 0;
    REG_WINOUT = 0;
}

void DisplayWindow_Set(u32 index, u32 value) {
    (void)index;
    (void)value;
}

void DisplayWindow_Get(void) {
    /* Returns window config - implicit r0 return */
}

s32 DisplayWindow_IsVisible(void) {
    return g_dispwin_active ? 1 : 0;
}

void DisplayWindow_Enable(void) {
    g_dispwin_active = 1;
}

void DisplayWindow_Disable(void) {
    g_dispwin_active = 0;
    REG_WININ = 0;
    REG_WINOUT = 0;
}

void DisplayWindow_SetLayerMask(u32 index, u32 value) {
    (void)index;
    g_window_layer_mask = value;
}
