#include "nds_types.h"
void Ring_Collect(void) { g_game_rings++; if (g_game_rings > 999) g_game_rings = 999; }
void Score_Add(s32 x, s32 y, u32 param) { (void)x; (void)y; g_game_score += param * g_scoremult_val; }
void Ring_LoseAll(void) { u32 lost = g_game_rings; g_game_rings = 0; if (lost > 0) RingScatter_Spawn(Player_GetX(), Player_GetY(), lost); }
u32 Ring_GetCount(void) { return g_game_rings; }
u32 Score_Get(void) { return g_game_score; }
