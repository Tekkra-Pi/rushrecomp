#include "nds_types.h"
static u32 s_state, s_timer;
void TitleCard_Init(void) { s_state = 1; s_timer = 0; g_titlecard_state = 1; g_titlecard_timer = 0; }
void TitleCard_Start(void) { TitleCard_Init(); }
s32 TitleCard_Update(void) {
    if (s_state == 0) return 0;
    s_timer++; g_titlecard_timer++;
    if (s_timer >= 180) { s_state = 0; g_titlecard_state = 0; return 1; }
    return 0;
}
void TitleCard_Draw(void) {
    if (s_state == 0) return;
    Display_PrintFixed(80, 80, "ZONE");
}
void TitleCard_Skip(void) { s_state = 0; g_titlecard_state = 0; }
u32 TitleCard_GetState(void) { return s_state; }
u32 TitleCard_GetTimer(void) { return s_timer; }
void TitleCard_Reset(void) { s_state = 0; s_timer = 0; g_titlecard_state = 0; g_titlecard_timer = 0; }
