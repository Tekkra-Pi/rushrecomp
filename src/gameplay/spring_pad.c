/* Gameplay level spring pad functions. */
#include "nds_types.h"


void SpringPad_Init(void) { g_springpad_count = 0; }

s32 SpringPad_Add(s32 x, s32 y, s32 force, u32 dir) {
    if (g_springpad_count >= 16) return -1;
    u32 i = g_springpad_count;
    g_springpads[i].x = x; g_springpads[i].y = y;
    g_springpads[i].force = force; g_springpads[i].dir = dir;
    g_springpads[i].anim = 0; g_springpads[i].active = 1;
    g_springpad_count++; return i;
}

void SpringPad_Update(void) {
    u32 i;
    for (i = 0; i < g_springpad_count; i++) {
        if (!g_springpads[i].active) continue;
        if (g_springpads[i].anim > 0) g_springpads[i].anim--;
    }
}

void SpringPad_Trig(u32 i) {
    if (i < g_springpad_count && g_springpads[i].active) {
        s32 vx = 0, vy = 0;
        switch (g_springpads[i].dir) {
            case 0: vy = -g_springpads[i].force; break;
            case 1: vy = g_springpads[i].force; break;
            case 2: vx = -g_springpads[i].force; break;
            case 3: vx = g_springpads[i].force; break;
        }
        Player_SetVelocity(vx, vy);
        g_springpads[i].anim = 16;
        SoundEffect_Play(0x22, 0x40, 0x100);
    }
}

void SpringPad_Remove(u32 i) { if (i < g_springpad_count) g_springpad_count--; }
u32 SpringPad_GetCount(void) { return g_springpad_count; }
void SpringPad_Clear(void) { g_springpad_count = 0; }
void SpringPad_Reset(void) { g_springpad_count = 0; }
