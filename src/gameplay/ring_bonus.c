#include "nds_types.h"
static u32 s_state, s_timer;
void RingBonus_Init(void) { s_state = 0; s_timer = 0; g_ringbonus_score = 0; }
void RingBonus_Start(void) { s_state = 1; s_timer = 0; g_ringbonus_score = g_game_rings * 100; }
s32 RingBonus_Update(void) {
    if (s_state == 0) return 0;
    s_timer++;
    if (s_timer >= 180) { s_state = 0; return 1; }
    return 0;
}
void RingBonus_Draw(void) { if (s_state) Display_PrintFixed(80, 80, "RING BONUS"); }
void RingBonus_Skip(void) { s_state = 0; }
u32 RingBonus_GetScore(void) { return g_ringbonus_score; }
u32 RingBonus_GetState(void) { return s_state; }
void RingBonus_Reset(void) { s_state = 0; s_timer = 0; }
