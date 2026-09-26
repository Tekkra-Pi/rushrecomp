/* Gameplay level score helper functions. */

#include "nds_types.h"

void ScoreHelper_Init(void) {
    g_score_helper_bonus = 0;
}

void ScoreHelper_Add(u32 score) {
    g_score_helper_bonus += score;
}

u32 ScoreHelper_Get(void) {
    return g_score_helper_bonus;
}

void ScoreHelper_Clear(void) {
    g_score_helper_bonus = 0;
}

void ScoreHelper_Reset(void) {
    g_score_helper_bonus = 0;
}
