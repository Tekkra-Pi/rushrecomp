/* Gameplay level ladder functions. */
#include "nds_types.h"


void Ladder_Init(void) { g_ladder_count = 0; }

s32 Ladder_Add(s32 x, s32 y, s32 height) {
    if (g_ladder_count >= 8) return -1;
    u32 i = g_ladder_count;
    g_ladders[i].x = x; g_ladders[i].y = y;
    g_ladders[i].height = height; g_ladders[i].active = 1;
    g_ladder_count++; return i;
}

void Ladder_Update(void) {
    u32 i;
    for (i = 0; i < g_ladder_count; i++) {
        if (!g_ladders[i].active) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_ladders[i].x;
        if (dx >= -0x400 && dx <= 0x400 && py >= g_ladders[i].y && py <= g_ladders[i].y + g_ladders[i].height) {
            if (Input_GetHeld() & KEY_UP) {
                Player_SetPosition(px, py - 0x100);
            } else if (Input_GetHeld() & KEY_DOWN) {
                Player_SetPosition(px, py + 0x100);
            }
        }
    }
}

void Ladder_Remove(u32 i) { if (i < g_ladder_count) g_ladder_count--; }
u32 Ladder_GetCount(void) { return g_ladder_count; }
void Ladder_Clear(void) { g_ladder_count = 0; }
void Ladder_Reset(void) { g_ladder_count = 0; }
