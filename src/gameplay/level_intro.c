#include "nds_types.h"
void LevelIntro_Init(void) { g_levelintro_state = 0; }
void LevelIntro_Start(void) { g_levelintro_state = 1; TitleCard_Init(); }
s32 LevelIntro_Update(void) {
    if (g_levelintro_state == 0) return 0;
    TitleCard_Update();
    if (g_key_pressed & (1 << 0)) { g_levelintro_state = 0; return 1; }
    return 0;
}
void LevelIntro_Draw(void) { TitleCard_Draw(); }
u32 LevelIntro_GetState(void) { return g_levelintro_state; }
void LevelIntro_Reset(void) { g_levelintro_state = 0; }
