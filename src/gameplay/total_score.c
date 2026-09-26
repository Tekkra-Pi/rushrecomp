#include "nds_types.h"
static u32 s_state, s_timer;
void TotalScore_Init(void) { s_state = 0; s_timer = 0; }
void TotalScore_Start(void) { s_state = 1; s_timer = 0; g_totscore_score = 0; }
s32 TotalScore_Update(void) {
    if (s_state == 0) return 0;
    s_timer++;
    if (g_totscore_score < g_game_score) { g_totscore_score += 100; if (g_totscore_score > g_game_score) g_totscore_score = g_game_score; }
    if (s_timer >= 300) { s_state = 0; return 1; }
    return 0;
}
void TotalScore_Draw(void) { if (s_state) Display_PrintFixed(80, 80, "TOTAL SCORE"); }
void TotalScore_Skip(void) { s_state = 0; }
u32 TotalScore_GetState(void) { return s_state; }
void TotalScore_Reset(void) { s_state = 0; s_timer = 0; }
