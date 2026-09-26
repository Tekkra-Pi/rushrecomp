/* Display scroll functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

void DisplayScroll_Init(void) {
    g_dscroll_x = 0;
    g_dscroll_y = 0;
    g_dscroll_oy = 0;
    g_dscroll_target_x = 0;
    g_dscroll_target_y = 0;
}

void DisplayScroll_GetPosition(void) {
}

void DisplayScroll_SetOffset(u32 index, u32 value) {
    (void)index;
    g_dscroll_x = value;
}

void DisplayScroll_SetBgOffset(u32 index, u32 value) {
    (void)index;
    g_dscroll_y = value;
}

void DisplayScroll_SetParallax(u32 index, u32 value) {
    (void)index;
    g_parallax_y = value;
}

void DisplayScroll_Reset(void) {
    g_dscroll_x = 0;
    g_dscroll_y = 0;
    g_dscroll_oy = 0;
    g_dscroll_target_x = 0;
    g_dscroll_target_y = 0;
}
