/* Gameplay level score multiplier functions. */
#include "nds_types.h"

void ScoreMult_Init(void) { g_scoremult_val = 1; g_scoremult_timer = 0; }

void ScoreMult_Set(u32 mult, u32 duration) {
    g_scoremult_val = mult; g_scoremult_timer = duration;
}

void ScoreMult_Update(void) {
    if (g_scoremult_timer > 0) {
        g_scoremult_timer--;
        if (g_scoremult_timer == 0) g_scoremult_val = 1;
    }
}

u32 ScoreMult_Get(void) { return g_scoremult_val; }
u32 ScoreMult_GetTimer(void) { return g_scoremult_timer; }
void ScoreMult_Reset(void) { g_scoremult_val = 1; g_scoremult_timer = 0; }
