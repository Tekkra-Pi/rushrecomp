#include "nds_types.h"
void ModeState_Init(void) { g_modestate_mode = 0; }
void ModeState_Set(u32 mode) { g_modestate_mode = mode; }
u32 ModeState_Get(void) { return g_modestate_mode; }
void ModeState_Update(void) { }
void ModeState_Reset(void) { g_modestate_mode = 0; }
