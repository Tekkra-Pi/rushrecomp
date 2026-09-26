/* Gameplay level time bonus functions. */

#include "nds_types.h"

void TimeBonus_Init(void) {
    g_timebonus_value = 0;
}

void TimeBonus_Start(void) {
    u32 time = g_timer_seconds + g_timer_minutes * 60;
    if (time < 60) g_timebonus_value = 10000;
    else if (time < 120) g_timebonus_value = 5000;
    else if (time < 180) g_timebonus_value = 2000;
    else if (time < 300) g_timebonus_value = 1000;
    else if (time < 600) g_timebonus_value = 500;
    else g_timebonus_value = 0;
}

s32 TimeBonus_Update(void) {
    if (g_timebonus_value > 0) {
        g_game_score += 100;
        g_timebonus_value -= 100;
        if (g_timebonus_value <= 0) {
            g_timebonus_value = 0;
            return 1;
        }
    }
    return 0;
}

u32 TimeBonus_GetValue(void) { return g_timebonus_value; }

void TimeBonus_Reset(void) {
    g_timebonus_value = 0;
}
