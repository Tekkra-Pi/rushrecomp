/* Gameplay ring functions. */
#include "nds_types.h"

void GameRings_Init(void) { g_game_rings = 0; g_rings_display = 0; }
void GameRings_Add(s32 x, s32 y, u32 param) {
    (void)x; (void)y;
    g_game_rings += param;
    if (g_game_rings > 999) g_game_rings = 999;
}
void GameRings_Remove(u32 index) { (void)index; if (g_game_rings > 0) g_game_rings--; }
void GameRings_LoseAll(void) {
    u32 lost = g_game_rings;
    g_game_rings = 0;
    if (lost > 0) RingScatter_Spawn(Player_GetX(), Player_GetY(), lost);
}
u32 GameRings_Get(void) { return g_game_rings; }
void GameRings_Set(u32 index, u32 value) { (void)index; g_game_rings = value; }
void GameRings_UpdateDisplay(void) {
    if (g_rings_display < g_game_rings) g_rings_display++;
    else if (g_rings_display > g_game_rings) g_rings_display--;
}
u32 GameRings_GetDisplay(void) { return g_rings_display; }
