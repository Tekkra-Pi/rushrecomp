/* Gameplay level AI actor functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);

#define AIACTOR_MAX 16

void AiActor_Init(void) { g_aiactor_count = 0; }

s32 AiActor_Add(s32 x, s32 y, u32 param) {
    if (g_aiactor_count >= AIACTOR_MAX) return -1;
    u32 i = g_aiactor_count;
    g_aiactors[i].x = x;
    g_aiactors[i].y = y;
    g_aiactors[i].start_x = x;
    g_aiactors[i].start_y = y;
    g_aiactors[i].end_x = x + 0x1000;
    g_aiactors[i].end_y = y;
    g_aiactors[i].speed = 0x10;
    g_aiactors[i].progress = 0;
    g_aiactors[i].forward = 1;
    g_aiactors[i].active = 1;
    g_aiactor_count++;
    return i;
}

void AiActor_Update(void) {
    u32 i;
    for (i = 0; i < g_aiactor_count; i++) {
        if (!g_aiactors[i].active) continue;
        if (g_aiactors[i].forward) {
            g_aiactors[i].progress += g_aiactors[i].speed;
            if (g_aiactors[i].progress >= 256) {
                g_aiactors[i].progress = 256;
                g_aiactors[i].forward = 0;
            }
        } else {
            if (g_aiactors[i].progress <= g_aiactors[i].speed) {
                g_aiactors[i].progress = 0;
                g_aiactors[i].forward = 1;
            } else {
                g_aiactors[i].progress -= g_aiactors[i].speed;
            }
        }
        g_aiactors[i].x = g_aiactors[i].start_x +
            (g_aiactors[i].end_x - g_aiactors[i].start_x) * g_aiactors[i].progress / 256;
        g_aiactors[i].y = g_aiactors[i].start_y +
            (g_aiactors[i].end_y - g_aiactors[i].start_y) * g_aiactors[i].progress / 256;
    }
}

void AiActor_Remove(u32 index) {
    if (index < g_aiactor_count) g_aiactors[index].active = 0;
}

s32 AiActor_GetX(u32 index) {
    if (index >= g_aiactor_count) return 0;
    return g_aiactors[index].x;
}

s32 AiActor_GetY(u32 index) {
    if (index >= g_aiactor_count) return 0;
    return g_aiactors[index].y;
}

u32 AiActor_GetType(void) { return 0; }

u32 AiActor_GetCount(void) { return g_aiactor_count; }

s32 AiActor_IsActive(u32 index) {
    if (index >= g_aiactor_count) return 0;
    return g_aiactors[index].active;
}

void AiActor_Clear(void) { g_aiactor_count = 0; }
void AiActor_Reset(void) { g_aiactor_count = 0; }
