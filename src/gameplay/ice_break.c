/* Gameplay level ice break functions. */
#include "nds_types.h"

extern void ExplosionFX_Spawn(s32 x, s32 y);

#define ICEBREAK_MAX 8

void IceBreak_Init(void) { }

s32 IceBreak_Add(s32 x, s32 y) {
    u32 i = 0;
    g_icebreaks[i].x = x;
    g_icebreaks[i].y = y;
    g_icebreaks[i].active = 1;
    return i;
}

void IceBreak_Hit(u32 index) {
    if (index >= ICEBREAK_MAX) return;
    if (g_icebreaks[index].active) {
        g_icebreaks[index].active = 0;
        ExplosionFX_Spawn(g_icebreaks[index].x, g_icebreaks[index].y);
    }
}

void IceBreak_Update(void) { }

void IceBreak_Remove(u32 index) {
    if (index < ICEBREAK_MAX) g_icebreaks[index].active = 0;
}

u32 IceBreak_GetCount(void) { return ICEBREAK_MAX; }

void IceBreak_Clear(void) {
    u32 i;
    for (i = 0; i < ICEBREAK_MAX; i++) g_icebreaks[i].active = 0;
}

void IceBreak_Reset(void) { IceBreak_Clear(); }
