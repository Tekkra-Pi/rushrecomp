/* Gameplay cutscene functions. */
#include "nds_types.h"

void Cutscene_Start(void) { g_cutscene_state = 1; g_cutscene_timer = 0; }
void Cutscene_ShowDialog(void) { g_dialog_state = 1; g_dialog_char_index = 0; }
s32 Cutscene_IsComplete(u32 index) { (void)index; return g_cutscene_state == 0 ? 1 : 0; }
u32 Cutscene_GetState(void) { return g_cutscene_state; }
u32 Cutscene_GetTimer(void) { return g_cutscene_timer; }
u32 Cutscene_GetPC(void) { return g_cutscene_timer; }
void Cutscene_Skip(void) { g_cutscene_state = 0; }
void Cutscene_Stop(void) { g_cutscene_state = 0; g_cutscene_timer = 0; g_dialog_state = 0; }
void Cutscene_Reset(void) { g_cutscene_state = 0; g_cutscene_timer = 0; }
