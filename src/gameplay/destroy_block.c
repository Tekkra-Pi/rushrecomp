/* Gameplay level destroy block functions. */

#include "nds_types.h"

extern void ExplosionFX_Spawn(s32 x, s32 y);

#define DESTROYBLOCK_MAX 16

void DestroyBlock_Init(void) { }

s32 DestroyBlock_Add(s32 x, s32 y, u32 param) {
    u32 i = 0;
    g_destroyblocks[i].x = x;
    g_destroyblocks[i].y = y;
    g_destroyblocks[i].active = 1;
    return i;
}

void DestroyBlock_Update(void) {
}

void DestroyBlock_Destroy(void) {
    u32 i;
    for (i = 0; i < DESTROYBLOCK_MAX; i++) {
        if (!g_destroyblocks[i].active) continue;
        g_destroyblocks[i].active = 0;
        ExplosionFX_Spawn(g_destroyblocks[i].x, g_destroyblocks[i].y);
    }
}

void DestroyBlock_Remove(u32 index) {
    if (index < DESTROYBLOCK_MAX) g_destroyblocks[index].active = 0;
}

u32 DestroyBlock_GetCount(void) { return DESTROYBLOCK_MAX; }

void DestroyBlock_Clear(void) {
    u32 i;
    for (i = 0; i < DESTROYBLOCK_MAX; i++) g_destroyblocks[i].active = 0;
}

void DestroyBlock_Reset(void) {
    u32 i;
    for (i = 0; i < DESTROYBLOCK_MAX; i++) g_destroyblocks[i].active = 0;
}
