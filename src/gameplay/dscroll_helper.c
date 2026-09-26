/* Gameplay level display scroll helper functions. */
#include "nds_types.h"

static u32 s_active; static s32 s_x, s_y, s_tx, s_ty, s_spd;

void DScrollHelper_Init(void) { s_active = 0; s_x = 0; s_y = 0; s_tx = 0; s_ty = 0; s_spd = 0; }
void DScrollHelper_Start(void) { s_active = 1; s_x = g_dscroll_x; s_y = g_dscroll_y; }
void DScrollHelper_Update(void) {
    if (!s_active) return;
    if (s_x < s_tx) { s_x += s_spd; if (s_x > s_tx) s_x = s_tx; }
    else if (s_x > s_tx) { s_x -= s_spd; if (s_x < s_tx) s_x = s_tx; }
    g_dscroll_x = (u32)s_x;
}
void DScrollHelper_Stop(void) { s_active = 0; }
s32 DScrollHelper_IsActive(u32 index) { (void)index; return s_active; }
void DScrollHelper_Reset(void) { DScrollHelper_Init(); }
