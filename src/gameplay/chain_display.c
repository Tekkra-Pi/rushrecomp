/* Gameplay chain display functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_chaindisp_visible;
static s32 s_chaindisp_x;
static s32 s_chaindisp_y;

void ChainDisplay_Init(void) {
    s_chaindisp_visible = 0;
    s_chaindisp_x = 0;
    s_chaindisp_y = 0;
    g_chaindisplay_visible = 0;
}

void ChainDisplay_SetPosition(u32 index, u32 value) {
    (void)index;
    g_chaindisplay_x = (s32)value;
}

void ChainDisplay_Draw(void) {
    if (!s_chaindisp_visible) return;
}

void ChainDisplay_Show(void) {
    s_chaindisp_visible = 1;
    g_chaindisplay_visible = 1;
}

void ChainDisplay_Hide(void) {
    s_chaindisp_visible = 0;
    g_chaindisplay_visible = 0;
}

s32 ChainDisplay_IsVisible(void) {
    return s_chaindisp_visible;
}

void ChainDisplay_GetPosition(void) {
}

void ChainDisplay_Reset(void) {
    s_chaindisp_visible = 0;
    s_chaindisp_x = 0;
    s_chaindisp_y = 0;
    g_chaindisplay_visible = 0;
}
