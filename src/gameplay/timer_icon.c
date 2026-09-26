#include "nds_types.h"
static u32 s_count;
void TimerIcon_Init(void) { s_count = 0; }
s32 TimerIcon_Add(s32 x, s32 y, u32 param) {
    (void)param;
    if (s_count >= 4) return -1;
    g_hud_anims[s_count].x = x; g_hud_anims[s_count].y = y;
    g_hud_anims[s_count].active = 1;
    s_count++; return s_count - 1;
}
void TimerIcon_Update(void) { }
void TimerIcon_Draw(void) { }
void TimerIcon_Remove(u32 index) { if (index < s_count) g_hud_anims[index].active = 0; }
u32 TimerIcon_GetCount(void) { return s_count; }
void TimerIcon_Clear(void) { s_count = 0; }
void TimerIcon_Reset(void) { s_count = 0; }
