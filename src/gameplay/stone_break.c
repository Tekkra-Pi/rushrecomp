/* Gameplay level stone break functions. */
#include "nds_types.h"

extern void ExplosionFX_Spawn(s32 x, s32 y);

void StoneBreak_Init(void) { }

s32 StoneBreak_Add(s32 x, s32 y) {
    u32 i = 0;
    g_stonebreaks[i].x = x;
    g_stonebreaks[i].y = y;
    g_stonebreaks[i].active = 1;
    return i;
}

void StoneBreak_Hit(u32 index) {
    if (index >= 8) return;
    if (g_stonebreaks[index].active) {
        g_stonebreaks[index].active = 0;
        ExplosionFX_Spawn(g_stonebreaks[index].x, g_stonebreaks[index].y);
    }
}

void StoneBreak_Update(void) { }

void StoneBreak_Remove(u32 index) {
    if (index < 8) g_stonebreaks[index].active = 0;
}

u32 StoneBreak_GetCount(void) { return 8; }

void StoneBreak_Clear(void) {
    u32 i;
    for (i = 0; i < 8; i++) g_stonebreaks[i].active = 0;
}

void StoneBreak_Reset(void) { StoneBreak_Clear(); }
