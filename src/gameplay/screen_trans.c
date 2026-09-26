#include "nds_types.h"
void ScreenTrans_Init(void) { g_screentrans_state = 0; }
void ScreenTrans_Start(void) { g_screentrans_state = 1; g_transition_timer = 0; }
s32 ScreenTrans_Update(void) {
    if (g_screentrans_state == 0) return 0;
    g_transition_timer++;
    if (g_transition_timer >= g_transition_duration) { g_screentrans_state = 0; return 1; }
    return 0;
}
u32 ScreenTrans_GetState(void) { return g_screentrans_state; }
void ScreenTrans_Reset(void) { g_screentrans_state = 0; g_transition_timer = 0; }
