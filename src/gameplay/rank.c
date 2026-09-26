/* Gameplay level rank functions. */

#include "nds_types.h"

void Rank_Init(void) {
    g_rank_helper_rank = 0;
    g_rank_helper_rings = 0;
    g_rank_helper_score = 0;
    g_rank_helper_time = 0;
}

void Rank_Evaluate(void) {
    u32 rank = 0;
    if (g_rank_helper_rings >= 200) rank += 2;
    else if (g_rank_helper_rings >= 100) rank += 1;
    if (g_rank_helper_score >= 500000) rank += 2;
    else if (g_rank_helper_score >= 200000) rank += 1;
    if (g_rank_helper_time <= 180) rank += 2;
    else if (g_rank_helper_time <= 300) rank += 1;
    if (rank >= 5) g_rank_helper_rank = 0;
    else if (rank >= 3) g_rank_helper_rank = 1;
    else if (rank >= 1) g_rank_helper_rank = 2;
    else g_rank_helper_rank = 3;
}

u32 Rank_GetRank(void) { return g_rank_helper_rank; }

void Rank_SetRings(u32 rings) { g_rank_helper_rings = rings; }
void Rank_SetScore(u32 score) { g_rank_helper_score = score; }
void Rank_SetTime(u32 time) { g_rank_helper_time = time; }

void Rank_Reset(void) {
    g_rank_helper_rank = 0;
    g_rank_helper_rings = 0;
    g_rank_helper_score = 0;
    g_rank_helper_time = 0;
}
