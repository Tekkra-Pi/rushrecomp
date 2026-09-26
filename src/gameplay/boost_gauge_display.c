/* Gameplay level boost gauge display functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

static u32 s_boostdisp_visible;
static s32 s_boostdisp_x;
static s32 s_boostdisp_y;

void BoostGaugeDisplay_Init(void) {
    s_boostdisp_visible = 0;
    s_boostdisp_x = 0;
    s_boostdisp_y = 0;
}

void BoostGaugeDisplay_SetPosition(u32 index, u32 value) {
    (void)index;
    s_boostdisp_x = (s32)value;
}

void BoostGaugeDisplay_Draw(void) {
    if (!s_boostdisp_visible) return;
}

void BoostGaugeDisplay_Show(void) {
    s_boostdisp_visible = 1;
    g_hudboost_visible = 1;
}

void BoostGaugeDisplay_Hide(void) {
    s_boostdisp_visible = 0;
    g_hudboost_visible = 0;
}

s32 BoostGaugeDisplay_IsVisible(void) {
    return s_boostdisp_visible;
}

void BoostGaugeDisplay_GetPosition(void) {
}

void BoostGaugeDisplay_Reset(void) {
    s_boostdisp_visible = 0;
    s_boostdisp_x = 0;
    s_boostdisp_y = 0;
}
