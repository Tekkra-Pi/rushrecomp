#include "nds_types.h"
static u32 s_state, s_timer;
void LevelClear_Init(void) { s_state = 0; s_timer = 0; }
void LevelClear_Start(void) { s_state = 1; s_timer = 0; }
s32 LevelClear_Update(void) {
    if (s_state == 0) return 0;
    s_timer++; if (s_timer >= 120) { s_state = 0; return 1; } return 0;
}
void LevelClear_Draw(void) { if (s_state) Display_PrintFixed(80, 80, "STAGE CLEAR!"); }
u32 LevelClear_GetState(void) { return s_state; }
u32 LevelClear_GetTimer(void) { return s_timer; }
void LevelClear_Reset(void) { s_state = 0; s_timer = 0; }
