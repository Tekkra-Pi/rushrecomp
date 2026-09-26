/* Gameplay level rail segment functions. */
#include "nds_types.h"

void RailSeg_Init(void) { g_railseg_count = 0; }

s32 RailSeg_Add(s32 x1, s32 y1, s32 x2, s32 y2) {
    if (g_railseg_count >= 32) return -1;
    u32 i = g_railseg_count;
    g_railsegs[i].x1 = x1; g_railsegs[i].y1 = y1;
    g_railsegs[i].x2 = x2; g_railsegs[i].y2 = y2;
    g_railsegs[i].active = 1;
    g_railseg_count++; return i;
}

void RailSeg_Remove(u32 i) { if (i < g_railseg_count) g_railseg_count--; }

s32 RailSeg_GetX1(u32 i) { return (i < g_railseg_count) ? g_railsegs[i].x1 : 0; }
s32 RailSeg_GetY1(u32 i) { return (i < g_railseg_count) ? g_railsegs[i].y1 : 0; }
s32 RailSeg_GetX2(u32 i) { return (i < g_railseg_count) ? g_railsegs[i].x2 : 0; }
s32 RailSeg_GetY2(u32 i) { return (i < g_railseg_count) ? g_railsegs[i].y2 : 0; }

s32 RailSeg_Nearest(s32 px, s32 py) {
    u32 i, best = 0xffffffff;
    s32 best_i = -1;
    for (i = 0; i < g_railseg_count; i++) {
        s32 dx = (g_railsegs[i].x1 + g_railsegs[i].x2) / 2 - px;
        s32 dy = (g_railsegs[i].y1 + g_railsegs[i].y2) / 2 - py;
        u32 d = (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);
        if (d < best) { best = d; best_i = i; }
    }
    return best_i;
}

u32 RailSeg_GetCount(void) { return g_railseg_count; }
void RailSeg_Clear(void) { g_railseg_count = 0; }
void RailSeg_Reset(void) { g_railseg_count = 0; }
