/* Gameplay ending functions. */
#include "nds_types.h"

void Ending_Init(void) { g_ending_state = 0; g_cutscene_timer = 0; }
void Ending_Start(void) { g_ending_state = 1; g_cutscene_timer = 0; }
s32 Ending_Update(void) {
    if (g_ending_state == 0) return 0;
    g_cutscene_timer++;
    if (g_cutscene_timer >= 600) { g_ending_state = 0; return 1; }
    return 0;
}
void Ending_Draw(void) { }
s32 Ending_IsComplete(u32 index) { (void)index; return g_ending_state == 0 ? 1 : 0; }
u32 Ending_GetState(void) { return g_ending_state; }
u32 Ending_GetStep(void) { return g_cutscene_timer / 60; }
u32 Ending_GetTimer(void) { return g_cutscene_timer; }
void Ending_Skip(void) { g_ending_state = 0; }
void Ending_Reset(void) { g_ending_state = 0; g_cutscene_timer = 0; }
