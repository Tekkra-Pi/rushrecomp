#include "nds_types.h"
static u32 s_count;
void ScoreIcon_Init(void) { s_count = 0; }
s32 ScoreIcon_Add(s32 x, s32 y, u32 param) {
    (void)param;
    if (s_count >= 4) return -1;
    g_hud_anims[s_count].x = x; g_hud_anims[s_count].y = y;
    g_hud_anims[s_count].active = 1;
    s_count++; return s_count - 1;
}
void ScoreIcon_Update(void) { }
void ScoreIcon_Draw(void) { }
void ScoreIcon_Remove(u32 index) { if (index < s_count) g_hud_anims[index].active = 0; }
u32 ScoreIcon_GetCount(void) { return s_count; }
void ScoreIcon_Clear(void) { s_count = 0; }
void ScoreIcon_Reset(void) { s_count = 0; }
