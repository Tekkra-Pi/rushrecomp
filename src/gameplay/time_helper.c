#include "nds_types.h"
static u32 s_active, s_elapsed, s_warning;
void TimeHelper_Init(void) { s_active = 0; s_elapsed = 0; s_warning = 0; g_time_helper_elapsed = 0; g_time_helper_warning = 0; }
void TimeHelper_Start(void) { s_active = 1; s_elapsed = 0; }
void TimeHelper_Update(void) {
    if (!s_active) return;
    s_elapsed++; g_time_helper_elapsed = s_elapsed;
    if (s_elapsed >= 240 && !s_warning) { s_warning = 1; g_time_helper_warning = 1; }
}
void TimeHelper_Stop(void) { s_active = 0; }
u32 TimeHelper_GetElapsed(void) { return s_elapsed; }
u32 TimeHelper_GetRemaining(void) { return s_elapsed < 300 ? 300 - s_elapsed : 0; }
u32 TimeHelper_GetSeconds(void) { return s_elapsed / 60; }
u32 TimeHelper_GetRemainingSeconds(void) { return TimeHelper_GetRemaining() / 60; }
void TimeHelper_PlayWarning(void) { SoundEffect_Play(0xd3, 0x40, 0x100); }
void TimeHelper_TimeUp(void) { s_active = 0; }
s32 TimeHelper_IsActive(u32 index) { (void)index; return s_active; }
s32 TimeHelper_IsWarning(void) { return s_warning; }
void TimeHelper_Reset(void) { s_active = 0; s_elapsed = 0; s_warning = 0; }
