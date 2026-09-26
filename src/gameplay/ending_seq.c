/* Gameplay level ending sequence functions. */
#include "nds_types.h"

void EndingSeq_Init(void) { g_ending_state = 0; g_cutscene_timer = 0; }
void EndingSeq_Start(void) { g_ending_state = 1; g_cutscene_timer = 0; }
s32 EndingSeq_Update(void) {
    if (g_ending_state == 0) return 0;
    g_cutscene_timer++;
    if (g_cutscene_timer >= 300) { g_ending_state = 0; return 1; }
    return 0;
}
void EndingSeq_Draw(void) { }
void EndingSeq_Skip(void) { g_ending_state = 0; }
u32 EndingSeq_GetState(void) { return g_ending_state; }
u32 EndingSeq_GetTimer(void) { return g_cutscene_timer; }
void EndingSeq_Reset(void) { g_ending_state = 0; g_cutscene_timer = 0; }
