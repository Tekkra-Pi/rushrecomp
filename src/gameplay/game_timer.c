/* Gameplay timer functions. */
#include "nds_types.h"

static u32 s_active;

void GameTimer_Init(void) { g_game_time = 0; s_active = 0; }
void GameTimer_Update(void) {
    if (!s_active) return;
    g_timer_frames++;
    if (g_timer_frames >= 60) {
        g_timer_frames = 0; g_timer_seconds++; g_game_time++;
        if (g_timer_seconds >= 60) { g_timer_seconds = 0; g_timer_minutes++; }
    }
}
void GameTimer_Pause(void) { s_active = 0; }
void GameTimer_Resume(void) { s_active = 1; }
u32 GameTimer_Get(void) { return g_game_time; }
u32 GameTimer_GetSeconds(void) { return g_timer_seconds; }
void GameTimer_Reset(void) { g_game_time = 0; g_timer_frames = 0; g_timer_seconds = 0; g_timer_minutes = 0; s_active = 0; }
