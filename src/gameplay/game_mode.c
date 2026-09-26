/* Gameplay level game mode functions. */
#include "nds_types.h"

void GameMode_Init(void) { g_modestate_mode = 0; g_gamemode_player = 0; }
void GameMode_Set(u32 index, u32 value) { (void)index; g_modestate_mode = value; }
u32 GameMode_Get(void) { return g_modestate_mode; }
void GameMode_SetPlayer(u32 index, u32 value) { (void)index; g_gamemode_player = value; }
u32 GameMode_GetPlayer(void) { return g_gamemode_player; }
void GameMode_Update(void) { }
void GameMode_Reset(void) { g_modestate_mode = 0; g_gamemode_player = 0; }
