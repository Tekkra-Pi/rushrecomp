#include "nds_types.h"
void HomingTarget_Init(void) { g_homingtarget_count = 0; }
s32 HomingTarget_Add(s32 x, s32 y, u32 type) {
    if (g_homingtarget_count >= 16) return -1;
    u32 i = g_homingtarget_count;
    g_homingtargs[i].x = x; g_homingtargs[i].y = y;
    g_homingtargs[i].type = type; g_homingtargs[i].active = 1;
    g_homingtarget_count++; return i;
}
s32 HomingTarget_GetNearest(s32 x, s32 y) {
    s32 best = -1; u32 best_dist = 0x7fffffff; u32 i;
    for (i = 0; i < g_homingtarget_count; i++) {
        if (!g_homingtargs[i].active) continue;
        s32 dx = g_homingtargs[i].x - x, dy = g_homingtargs[i].y - y;
        u32 dist = (u32)(dx * dx + dy * dy);
        if (dist < best_dist) { best_dist = dist; best = (s32)i; }
    }
    return best;
}
void HomingTarget_Remove(u32 index) { if (index < g_homingtarget_count) g_homingtargs[index].active = 0; }
u32 HomingTarget_GetCount(void) { return g_homingtarget_count; }
void HomingTarget_Clear(void) { g_homingtarget_count = 0; }
void HomingTarget_Reset(void) { g_homingtarget_count = 0; }
