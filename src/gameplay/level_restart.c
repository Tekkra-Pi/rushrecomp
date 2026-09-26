#include "nds_types.h"
void LevelRestart_Init(void) { }
void LevelRestart_Restart(void) { Player_Respawn(g_respawn_x, g_respawn_y); Enemy_Init(); GameRings_Init(); GameTimer_Init(); }
void LevelRestart_FromCheckpoint(void) { Player_Respawn(g_respawn_x, g_respawn_y); }
void LevelRestart_Reset(void) { }
