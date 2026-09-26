/* Gameplay level vine functions. */
#include "nds_types.h"


void Vine_Init(void) { g_vine_count = 0; }

s32 Vine_Add(s32 x, s32 y, s32 height) {
    if (g_vine_count >= 8) return -1;
    u32 i = g_vine_count;
    g_vines[i].x = x; g_vines[i].y = y;
    g_vines[i].height = height; g_vines[i].active = 1;
    g_vine_count++; return i;
}

void Vine_Update(void) {
    u32 i;
    for (i = 0; i < g_vine_count; i++) {
        if (!g_vines[i].active) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_vines[i].x;
        if (dx >= -0x400 && dx <= 0x400 && py >= g_vines[i].y && py <= g_vines[i].y + g_vines[i].height) {
            if (Input_GetHeld() & KEY_UP) {
                Player_SetPosition(px, py - 0x100);
            } else if (Input_GetHeld() & KEY_DOWN) {
                Player_SetPosition(px, py + 0x100);
            }
        }
    }
}

void Vine_Remove(u32 i) { if (i < g_vine_count) g_vine_count--; }
u32 Vine_GetCount(void) { return g_vine_count; }
void Vine_Clear(void) { g_vine_count = 0; }
void Vine_Reset(void) { g_vine_count = 0; }
