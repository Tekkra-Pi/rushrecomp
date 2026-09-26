/* Gameplay score functions. */
#include "nds_types.h"

void GameScore_Init(void) { g_game_score = 0; g_score_display = 0; }
void GameScore_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y;
    g_game_score += param * g_scoremult_val;
    if (g_game_score > 9999999) g_game_score = 9999999;
}
void GameScore_Subtract(void) { if (g_game_score >= 100) g_game_score -= 100; else g_game_score = 0; }
u32 GameScore_Get(void) { return g_game_score; }
u32 GameScore_GetHigh(void) { return g_bestrecord_score; }
void GameScore_Reset(void) { g_game_score = 0; }
void GameScore_UpdateDisplay(void) {
    if (g_score_display < g_game_score) { g_score_display += 100; if (g_score_display > g_game_score) g_score_display = g_game_score; }
    else if (g_score_display > g_game_score) { g_score_display -= 100; if (g_score_display < g_game_score) g_score_display = g_game_score; }
}
u32 GameScore_GetDisplay(void) { return g_score_display; }
