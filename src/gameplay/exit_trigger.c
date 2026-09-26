/* Gameplay level exit trigger functions. */
#include "nds_types.h"


void ExitTrigger_Init(void) { g_exittrigger_count = 0; }

s32 ExitTrigger_Add(s32 x, s32 y, s32 w, s32 h, u32 dest_zone, s32 dest_x, s32 dest_y) {
    if (g_exittrigger_count >= 4) return -1;
    u32 i = g_exittrigger_count;
    g_exittriggers[i].x = x; g_exittriggers[i].y = y;
    g_exittriggers[i].w = w; g_exittriggers[i].h = h;
    g_exittriggers[i].dest_zone = dest_zone;
    g_exittriggers[i].dest_x = dest_x; g_exittriggers[i].dest_y = dest_y;
    g_exittriggers[i].triggered = 0; g_exittriggers[i].active = 1;
    g_exittrigger_count++; return i;
}

void ExitTrigger_Update(void) {
    u32 i;
    for (i = 0; i < g_exittrigger_count; i++) {
        if (!g_exittriggers[i].active || g_exittriggers[i].triggered) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        if (px >= g_exittriggers[i].x && px <= g_exittriggers[i].x + g_exittriggers[i].w &&
            py >= g_exittriggers[i].y && py <= g_exittriggers[i].y + g_exittriggers[i].h) {
            g_exittriggers[i].triggered = 1;
            LevelWarp_Start(g_exittriggers[i].dest_zone, g_exittriggers[i].dest_x, g_exittriggers[i].dest_y);
        }
    }
}

void ExitTrigger_Remove(u32 i) { if (i < g_exittrigger_count) g_exittrigger_count--; }
u32 ExitTrigger_GetCount(void) { return g_exittrigger_count; }
void ExitTrigger_Clear(void) { g_exittrigger_count = 0; }
void ExitTrigger_Reset(void) { g_exittrigger_count = 0; }
