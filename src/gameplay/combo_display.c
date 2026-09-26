/* Gameplay level combo display functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_combodisp_visible;
static s32 s_combodisp_x;
static s32 s_combodisp_y;

void ComboDisplay_Init(void) {
    s_combodisp_visible = 0;
    s_combodisp_x = 0;
    s_combodisp_y = 0;
    g_combodisplay_visible = 0;
}

void ComboDisplay_SetPosition(u32 index, u32 value) {
    (void)index;
    g_combodisplay_x = (s32)value;
}

void ComboDisplay_Draw(void) {
    if (!s_combodisp_visible) return;
}

void ComboDisplay_Show(void) {
    s_combodisp_visible = 1;
    g_combodisplay_visible = 1;
}

void ComboDisplay_Hide(void) {
    s_combodisp_visible = 0;
    g_combodisplay_visible = 0;
}

s32 ComboDisplay_IsVisible(void) {
    return s_combodisp_visible;
}

void ComboDisplay_GetPosition(void) {
}

void ComboDisplay_Reset(void) {
    s_combodisp_visible = 0;
    s_combodisp_x = 0;
    s_combodisp_y = 0;
    g_combodisplay_visible = 0;
}
