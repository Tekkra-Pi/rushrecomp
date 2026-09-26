/* Gameplay level ring icon functions. */
#include "nds_types.h"


void RingIcon_Init(void) { g_ringicon_count = 0; }

s32 RingIcon_Add(s32 x, s32 y) {
    if (g_ringicon_count >= 4) return -1;
    u32 i = g_ringicon_count;
    g_ringicons[i].x = x; g_ringicons[i].y = y;
    g_ringicons[i].active = 1;
    g_ringicon_count++; return i;
}

void RingIcon_Update(void) {
    u32 i;
    for (i = 0; i < g_ringicon_count; i++) {
        if (!g_ringicons[i].active) continue;
        g_ringicons[i].y += Math_Sin(i * 128) / 256;
    }
}

void RingIcon_Draw(void) {
    u32 i;
    for (i = 0; i < g_ringicon_count; i++) {
        if (g_ringicons[i].active) {
            OAM_AddSprite(g_ringicons[i].x >> 8, g_ringicons[i].y >> 8, 0x390, 0x1000);
        }
    }
}

void RingIcon_Remove(u32 i) { if (i < g_ringicon_count) g_ringicon_count--; }
u32 RingIcon_GetCount(void) { return g_ringicon_count; }
void RingIcon_Clear(void) { g_ringicon_count = 0; }
void RingIcon_Reset(void) { g_ringicon_count = 0; }
