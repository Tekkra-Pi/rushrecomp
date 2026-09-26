/* Gameplay results functions. */
#include "nds_types.h"

static u32 s_state, s_timer;

void Results_Init(void) { s_state = 0; s_timer = 0; }
void Results_Start(void) {
    s_state = 1; s_timer = 0;
    g_results_rings = g_game_rings; g_results_score = g_game_score;
    g_results_time = g_game_time;
}
s32 Results_Update(void) {
    if (s_state == 0) return 0;
    s_timer++;
    if (s_timer >= 300) { s_state = 0; return 1; }
    return 0;
}
void Results_Draw(void) { if (s_state) Display_PrintFixed(80, 40, "RESULTS"); }
s32 Results_IsComplete(u32 index) { (void)index; return s_state == 0 ? 1 : 0; }
u32 Results_GetRank(void) { return g_results_rank; }
u32 Results_GetScore(void) { return g_results_score; }
u32 Results_GetTime(void) { return g_results_time; }
u32 Results_GetRings(void) { return g_results_rings; }
u32 Results_GetState(void) { return s_state; }
void Results_Reset(void) { s_state = 0; s_timer = 0; }
