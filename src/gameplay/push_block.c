/* Gameplay level push block functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);
extern void Player_SetPosition(s32 x, s32 y);

#define PUSHBLOCK_MAX 16

void PushBlock_Init(void) { }

s32 PushBlock_Add(s32 x, s32 y, u32 param) {
    u32 i = 0;
    g_pushblocks[i].x = x;
    g_pushblocks[i].y = y;
    g_pushblocks[i].w = 0x400;
    g_pushblocks[i].h = 0x400;
    g_pushblocks[i].vx = 0;
    g_pushblocks[i].vy = 0;
    g_pushblocks[i].pushing = 0;
    g_pushblocks[i].active = 1;
    g_pushblock_count++;
    return i;
}

void PushBlock_Update(void) {
    u32 i;
    for (i = 0; i < g_pushblock_count; i++) {
        if (!g_pushblocks[i].active) continue;
        g_pushblocks[i].x += g_pushblocks[i].vx;
        g_pushblocks[i].y += g_pushblocks[i].vy;
        g_pushblocks[i].vx = 0;
        g_pushblocks[i].vy = 0;
    }
}

void PushBlock_Remove(u32 index) {
    if (index < g_pushblock_count) g_pushblocks[index].active = 0;
}

u32 PushBlock_GetCount(void) { return g_pushblock_count; }

void PushBlock_Clear(void) { g_pushblock_count = 0; }
void PushBlock_Reset(void) { g_pushblock_count = 0; }
