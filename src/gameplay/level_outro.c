#include "nds_types.h"
void LevelOutro_Init(void) { g_leveloutro_state = 0; }
void LevelOutro_Start(void) { g_leveloutro_state = 1; g_cutscene_timer = 0; }
s32 LevelOutro_Update(void) {
    if (g_leveloutro_state == 0) return 0;
    g_cutscene_timer++;
    if (g_cutscene_timer >= 180) { g_leveloutro_state = 0; return 1; }
    return 0;
}
void LevelOutro_Draw(void) { }
u32 LevelOutro_GetState(void) { return g_leveloutro_state; }
void LevelOutro_Reset(void) { g_leveloutro_state = 0; g_cutscene_timer = 0; }
