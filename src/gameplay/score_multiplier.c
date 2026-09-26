#include "nds_types.h"
void ScoreMultiplier_Init(void) { g_scoremult_val = 1; g_scoremult_timer = 0; }
void ScoreMultiplier_Add(s32 x, s32 y, u32 param) { (void)x; (void)y; g_scoremult_val += param; if (g_scoremult_val > 8) g_scoremult_val = 8; g_scoremult_timer = 300; }
void ScoreMultiplier_Update(void) {
    if (g_scoremult_timer > 0) { g_scoremult_timer--; if (g_scoremult_timer == 0) g_scoremult_val = 1; }
}
void ScoreMultiplier_Reset(void) { g_scoremult_val = 1; g_scoremult_timer = 0; }
u32 ScoreMultiplier_Get(void) { return g_scoremult_val; }
s32 ScoreMultiplier_IsActive(u32 index) { (void)index; return g_scoremult_val > 1 ? 1 : 0; }
u32 ScoreMultiplier_GetTimer(void) { return g_scoremult_timer; }
u32 ScoreMultiplier_GetProgress(void) { return g_scoremult_timer; }
u32 ScoreMultiplier_GetMax(void) { return 8; }
void ScoreMultiplier_SetMax(u32 index, u32 value) { (void)index; (void)value; }
u32 ScoreMultiplier_Apply(void) { return g_scoremult_val; }
