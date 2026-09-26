#include "nds_types.h"
void LevelTransition_Init(void) { g_leveltrans_state = 0; g_transition_timer = 0; }
void LevelTransition_Start(void) { g_leveltrans_state = 1; g_transition_timer = 0; }
s32 LevelTransition_Update(void) {
    if (g_leveltrans_state == 0) return 0;
    g_transition_timer++;
    if (g_transition_timer >= 60) { g_leveltrans_state = 0; return 1; }
    return 0;
}
void LevelTransition_Stop(void) { g_leveltrans_state = 0; }
u32 LevelTransition_GetState(void) { return g_leveltrans_state; }
u32 LevelTransition_GetTimer(void) { return g_transition_timer; }
void LevelTransition_Reset(void) { g_leveltrans_state = 0; g_transition_timer = 0; }
