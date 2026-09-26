#include "nds_types.h"
void HomingTarg_Init(void) { g_homingtarget_count = 0; }
s32 HomingTarg_Add(s32 x, s32 y, u32 type) {
    if (g_homingtarget_count >= 16) return -1;
    u32 i = g_homingtarget_count;
    g_homingtargs[i].x = x; g_homingtargs[i].y = y;
    g_homingtargs[i].type = type; g_homingtargs[i].active = 1;
    g_homingtarget_count++; return i;
}
void HomingTarg_Update(void) { }
void HomingTarg_Remove(u32 index) { if (index < g_homingtarget_count) g_homingtargs[index].active = 0; }
u32 HomingTarg_GetCount(void) { return g_homingtarget_count; }
void HomingTarg_Clear(void) { g_homingtarget_count = 0; }
void HomingTarg_Reset(void) { g_homingtarget_count = 0; }
