#include "nds_types.h"
static u32 s_count;
void HurtboxHelper_Init(void) { s_count = 0; }
s32 HurtboxHelper_Add(s32 x, s32 y, s32 w, s32 h, void *owner) {
    if (s_count >= 16) return -1;
    g_hurtboxhs[s_count].x = x; g_hurtboxhs[s_count].y = y;
    g_hurtboxhs[s_count].width = w; g_hurtboxhs[s_count].height = h;
    g_hurtboxhs[s_count].owner = owner; g_hurtboxhs[s_count].active = 1;
    s_count++; return s_count - 1;
}
void HurtboxHelper_Update(void) { }
void HurtboxHelper_Remove(u32 index) { if (index < s_count) g_hurtboxhs[index].active = 0; }
u32 HurtboxHelper_GetCount(void) { return s_count; }
void HurtboxHelper_Clear(void) { s_count = 0; }
void HurtboxHelper_Reset(void) { s_count = 0; }
