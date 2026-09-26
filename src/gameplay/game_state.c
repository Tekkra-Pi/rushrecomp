/* Gameplay level game state functions. */
#include "nds_types.h"

void GameState_Init(void) { g_gamestate_state = 0; g_gamestate_prev = 0; }
void GameState_Set(u32 index, u32 value) {
    (void)index; g_gamestate_prev = g_gamestate_state; g_gamestate_state = value;
}
u32 GameState_Get(void) { return g_gamestate_state; }
u32 GameState_GetPrev(void) { return g_gamestate_prev; }
void GameState_Update(void) { }
void GameState_Reset(void) { g_gamestate_state = 0; g_gamestate_prev = 0; }
