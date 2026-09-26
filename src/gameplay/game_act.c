/* Gameplay act functions. */
#include "nds_types.h"

static u32 CalculateRank(void) {
    u32 sc = g_game_score;
    if (sc >= 100000) return 4;
    if (sc >= 50000) return 3;
    if (sc >= 20000) return 2;
    if (sc >= 5000) return 1;
    return 0;
}

void GameAct_Init(void) { g_act_completed = 0; g_act_rank = 0; }
void GameAct_Complete(void) { g_act_completed = 1; g_act_rank = CalculateRank(); }
u32 GameAct_CalculateRank(void) { return CalculateRank(); }
u32 GameAct_GetRank(void) { return g_act_rank; }
s32 GameAct_IsComplete(u32 index) { (void)index; return g_act_completed; }
u32 GameAct_GetScore(void) { return g_game_score; }
u32 GameAct_GetTime(void) { return g_game_time; }
void GameAct_SaveResults(void) { }
void GameAct_Next(void) { g_current_act++; }
