/* Gameplay level signpost functions. */
#include "nds_types.h"

void Signpost_Init(void) { g_signpost_count = 0; }

s32 Signpost_Add(s32 x, s32 y) {
    if (g_signpost_count >= 2) return -1;
    u32 i = g_signpost_count;
    g_signposts[i].x = x; g_signposts[i].y = y;
    g_signposts[i].timer = 0; g_signposts[i].active = 1;
    g_signpost_count++; return i;
}

void Signpost_Update(void) {
    u32 i;
    for (i = 0; i < g_signpost_count; i++) {
        if (!g_signposts[i].active) continue;
        g_signposts[i].timer++;
    }
}

void Signpost_Remove(u32 i) { if (i < g_signpost_count) g_signpost_count--; }
u32 Signpost_GetCount(void) { return g_signpost_count; }
void Signpost_Clear(void) { g_signpost_count = 0; }
void Signpost_Reset(void) { g_signpost_count = 0; }
