/* Gameplay level game over screen functions. */
#include "nds_types.h"

static u32 s_state, s_timer;

void GameOver_Init(void) { s_state = 0; s_timer = 0; }
void GameOver_Start(void) { s_state = 1; s_timer = 0; }
s32 GameOver_Update(void) {
    if (s_state == 0) return 0;
    s_timer++;
    if (s_timer >= 180) { s_state = 0; return 1; }
    return 0;
}
void GameOver_Draw(void) { if (s_state) Display_PrintFixed(80, 80, "GAME OVER"); }
u32 GameOver_GetState(void) { return s_state; }
u32 GameOver_GetTimer(void) { return s_timer; }
void GameOver_Reset(void) { s_state = 0; s_timer = 0; }
