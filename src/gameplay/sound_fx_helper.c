#include "nds_types.h"
void SoundFxHelper_Init(void) { g_sfxhelper_last = 0; }
void SoundFxHelper_PlayTimed(void) { }
void SoundFxHelper_Update(void) { }
void SoundFxHelper_Stop(void) { }
u32 SoundFxHelper_GetLast(void) { return g_sfxhelper_last; }
void SoundFxHelper_Reset(void) { g_sfxhelper_last = 0; }
