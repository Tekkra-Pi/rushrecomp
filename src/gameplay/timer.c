#include "nds_types.h"
static u32 s_active;
void Timer_Init(void) { g_time = 0; s_active = 0; }
void Timer_Start(void) { s_active = 1; }
void Timer_Stop(void) { s_active = 0; }
void Timer_Update(void) {
    if (!s_active) return;
    g_timer_frames++;
    if (g_timer_frames >= 60) { g_timer_frames = 0; g_timer_seconds++; g_time++; if (g_timer_seconds >= 60) { g_timer_seconds = 0; g_timer_minutes++; } }
}
u32 Timer_Get(void) { return g_time; }
u32 Timer_GetSeconds(void) { return g_timer_seconds; }
u32 Timer_IsActive(u32 index) { (void)index; return s_active; }
s32 Timer_IsExpired(void) { return g_time >= 300 ? 1 : 0; }
void Timer_Reset(void) { g_time = 0; g_timer_frames = 0; g_timer_seconds = 0; g_timer_minutes = 0; s_active = 0; }
