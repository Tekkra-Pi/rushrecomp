#include "nds_types.h"
void LevelSave_Init(void) { g_levelsave_rings = 0; g_levelsave_score = 0; }
void LevelSave_Save(void) { g_levelsave_rings = g_game_rings; g_levelsave_score = g_game_score; }
void LevelSave_Load(void) { g_game_rings = g_levelsave_rings; g_game_score = g_levelsave_score; }
void LevelSave_Clear(void) { g_levelsave_rings = 0; g_levelsave_score = 0; }
void LevelSave_Reset(void) { LevelSave_Init(); }
