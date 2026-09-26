#include "nds_types.h"
void ScreenTransition_Init(void) { g_screentrans_state = 0; g_transition_timer = 0; }
void ScreenTransition_Start(void) { g_screentrans_state = 1; g_transition_timer = 0; }
s32 ScreenTransition_Update(void) {
    if (g_screentrans_state == 0) return 0;
    g_transition_timer++;
    if (g_transition_timer >= 60) { g_screentrans_state = 0; return 1; }
    return 0;
}
void ScreenTransition_Draw(void) { }
void ScreenTransition_Stop(void) { g_screentrans_state = 0; }
s32 ScreenTransition_IsActive(u32 index) { (void)index; return g_screentrans_state; }
u32 ScreenTransition_GetType(void) { return g_transition_type; }
u32 ScreenTransition_GetProgress(void) { return g_transition_timer; }
void ScreenTransition_Reset(void) { g_screentrans_state = 0; g_transition_timer = 0; }
