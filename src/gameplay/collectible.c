/* Gameplay level collectible functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);

#define COLLECTIBLE_MAX 16

void Collectible_Init(void) { }

s32 Collectible_Add(s32 x, s32 y, u32 param) {
    u32 i = 0;
    g_enemspawns[i].x = x;
    g_enemspawns[i].y = y;
    g_enemspawns[i].type = param;
    g_enemspawns[i].active = 1;
    return i;
}

void Collectible_Update(void) {
    u32 i;
    for (i = 0; i < COLLECTIBLE_MAX; i++) {
        if (!g_enemspawns[i].active) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        s32 dx = px - g_enemspawns[i].x;
        s32 dy = py - g_enemspawns[i].y;
        if (dx >= -0x200 && dx <= 0x200 && dy >= -0x200 && dy <= 0x200) {
            g_enemspawns[i].active = 0;
            g_game_rings++;
        }
    }
}

void Collectible_Remove(u32 index) {
    if (index < COLLECTIBLE_MAX) g_enemspawns[index].active = 0;
}

u32 Collectible_GetCount(void) { return COLLECTIBLE_MAX; }

void Collectible_Clear(void) {
    u32 i;
    for (i = 0; i < COLLECTIBLE_MAX; i++) g_enemspawns[i].active = 0;
}

void Collectible_Reset(void) { Collectible_Clear(); }
