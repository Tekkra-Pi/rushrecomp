/* Gameplay lives functions. */
#include "nds_types.h"

void GameLives_Init(void) { g_game_lives = 3; }
void GameLives_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y;
    g_game_lives += param;
    if (g_game_lives > g_life_max) g_game_lives = g_life_max;
}
s32 GameLives_Remove(u32 index) { (void)index; if (g_game_lives > 0) g_game_lives--; return (s32)g_game_lives; }
u32 GameLives_Get(void) { return g_game_lives; }
void GameLives_Set(u32 index, u32 value) { (void)index; g_game_lives = value; }
u32 GameLives_GetMax(void) { return g_life_max; }
void GameLives_SetMax(u32 index, u32 value) { (void)index; g_life_max = value; }
s32 GameLives_CheckExtra(void) {
    if (g_game_score >= g_extra_life_pending) {
        g_extra_life_pending += 50000; g_game_lives++; return 1;
    }
    return 0;
}
