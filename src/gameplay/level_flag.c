#include "nds_types.h"
void LevelFlag_Init(void) { g_levelflag_count = 0; }
void LevelFlag_Set(u32 index, u32 value) { if (index < 32) g_evtflags[index] = value; }
u32 LevelFlag_Get(u32 index) { return index < 32 ? g_evtflags[index] : 0; }
void LevelFlag_Clear(u32 index) { if (index < 32) g_evtflags[index] = 0; }
void LevelFlag_Reset(void) { u32 i; for (i = 0; i < 32; i++) g_evtflags[i] = 0; }
