/* Gameplay level impact functions. */
#include "nds_types.h"

#define IMPACT_MAX 8

void Impact_Init(void) { }

s32 Impact_Spawn(s32 x, s32 y, u32 dir) {
    u32 i;
    for (i = 0; i < IMPACT_MAX; i++) {
        if (g_impacts[i].active) continue;
        g_impacts[i].x = x;
        g_impacts[i].y = y;
        g_impacts[i].direction = dir;
        g_impacts[i].frame = 0;
        g_impacts[i].timer = 0;
        g_impacts[i].active = 1;
        return i;
    }
    return -1;
}

void Impact_Update(void) {
    u32 i;
    for (i = 0; i < IMPACT_MAX; i++) {
        if (!g_impacts[i].active) continue;
        g_impacts[i].timer++;
        if (g_impacts[i].timer >= 10) {
            g_impacts[i].frame++;
            g_impacts[i].timer = 0;
            if (g_impacts[i].frame >= 4) g_impacts[i].active = 0;
        }
    }
}

void Impact_Draw(void) { }

void Impact_Remove(u32 index) {
    if (index < IMPACT_MAX) g_impacts[index].active = 0;
}

u32 Impact_GetCount(void) { return IMPACT_MAX; }

void Impact_Clear(void) {
    u32 i;
    for (i = 0; i < IMPACT_MAX; i++) g_impacts[i].active = 0;
}

void Impact_Reset(void) { Impact_Clear(); }
