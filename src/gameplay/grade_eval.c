/* Gameplay level grade evaluation functions. */
#include "nds_types.h"

void GradeEval_Init(void) {
    g_gradeeval_grade = 0;
}

u32 GradeEval_Evaluate(void) {
    u32 grade = 0;
    if (g_game_rings >= 200) grade += 2;
    else if (g_game_rings >= 100) grade += 1;
    if (g_game_score >= 500000) grade += 2;
    else if (g_game_score >= 200000) grade += 1;
    if (grade >= 3) g_gradeeval_grade = 0;
    else if (grade >= 2) g_gradeeval_grade = 1;
    else if (grade >= 1) g_gradeeval_grade = 2;
    else g_gradeeval_grade = 3;
    return g_gradeeval_grade;
}

u32 GradeEval_Get(void) { return g_gradeeval_grade; }

void GradeEval_Reset(void) { g_gradeeval_grade = 0; }
