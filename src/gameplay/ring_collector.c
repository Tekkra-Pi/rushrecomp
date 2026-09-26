/* Gameplay level ring collector functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);

#define RINGCOLLECT_MAX 16

void RingCollector_Init(void) { }

s32 RingCollector_Add(s32 x, s32 y, u32 num_rings) {
    u32 i = 0;
    g_ringcolls[i].x = x;
    g_ringcolls[i].y = y;
    g_ringcolls[i].num_rings = num_rings;
    g_ringcolls[i].collected = 0;
    g_ringcolls[i].active = 1;
    return i;
}

void RingCollector_Update(void) {
    u32 i;
    for (i = 0; i < RINGCOLLECT_MAX; i++) {
        if (!g_ringcolls[i].active || g_ringcolls[i].collected) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - g_ringcolls[i].x;
        s32 dy = py - g_ringcolls[i].y;
        if (dx >= -0x300 && dx <= 0x300 && dy >= -0x300 && dy <= 0x300) {
            g_ringcolls[i].collected = 1;
            g_game_rings += g_ringcolls[i].num_rings;
        }
    }
}

void RingCollector_Remove(u32 index) {
    if (index < RINGCOLLECT_MAX) g_ringcolls[index].active = 0;
}

u32 RingCollector_GetCount(void) { return RINGCOLLECT_MAX; }

void RingCollector_Clear(void) {
    u32 i;
    for (i = 0; i < RINGCOLLECT_MAX; i++) g_ringcolls[i].active = 0;
}

void RingCollector_Reset(void) { RingCollector_Clear(); }
