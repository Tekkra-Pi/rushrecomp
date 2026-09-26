/* Gameplay level pipe entry functions. */
#include "nds_types.h"


void PipeEntry_Init(void) { g_pipeentry_count = 0; }

s32 PipeEntry_Add(s32 x, s32 y, s32 dest_x, s32 dest_y) {
    if (g_pipeentry_count >= 8) return -1;
    u32 i = g_pipeentry_count;
    g_pipeentries[i].x = x; g_pipeentries[i].y = y;
    g_pipeentries[i].dest_x = dest_x; g_pipeentries[i].dest_y = dest_y;
    g_pipeentries[i].active = 1;
    g_pipeentry_count++; return i;
}

void PipeEntry_Update(void) {
    u32 i;
    for (i = 0; i < g_pipeentry_count; i++) {
        if (!g_pipeentries[i].active) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        s32 dx = px - g_pipeentries[i].x;
        s32 dy = py - g_pipeentries[i].y;
        if (dx >= -0x400 && dx <= 0x400 && dy >= -0x400 && dy <= 0x400) {
            if (Input_GetHeld() & KEY_DOWN) {
                Player_SetPosition(g_pipeentries[i].dest_x, g_pipeentries[i].dest_y);
                SoundEffect_Play(0x26, 0x40, 0x100);
            }
        }
    }
}

void PipeEntry_Remove(u32 i) { if (i < g_pipeentry_count) g_pipeentry_count--; }
u32 PipeEntry_GetCount(void) { return g_pipeentry_count; }
void PipeEntry_Clear(void) { g_pipeentry_count = 0; }
void PipeEntry_Reset(void) { g_pipeentry_count = 0; }
