/* Gameplay level timer zone functions. */
#include "nds_types.h"


void TimerZone_Init(void) { g_timerzone_count = 0; }

s32 TimerZone_Add(s32 x, s32 y, s32 w, s32 h, u32 time_limit) {
    if (g_timerzone_count >= 4) return -1;
    u32 i = g_timerzone_count;
    g_timerzones[i].x = x; g_timerzones[i].y = y;
    g_timerzones[i].w = w; g_timerzones[i].h = h;
    g_timerzones[i].time_limit = time_limit;
    g_timerzones[i].entered = 0; g_timerzones[i].active = 1;
    g_timerzone_count++; return i;
}

void TimerZone_Update(void) {
    u32 i;
    for (i = 0; i < g_timerzone_count; i++) {
        if (!g_timerzones[i].active) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        if (px >= g_timerzones[i].x && px <= g_timerzones[i].x + g_timerzones[i].w &&
            py >= g_timerzones[i].y && py <= g_timerzones[i].y + g_timerzones[i].h) {
            if (!g_timerzones[i].entered) {
                g_timerzones[i].entered = 1;
                Timer_Start(g_timerzones[i].time_limit);
            }
        } else {
            if (g_timerzones[i].entered) {
                g_timerzones[i].entered = 0;
                Timer_Stop();
            }
        }
    }
}

void TimerZone_Remove(u32 i) { if (i < g_timerzone_count) g_timerzone_count--; }
u32 TimerZone_GetCount(void) { return g_timerzone_count; }
void TimerZone_Clear(void) { g_timerzone_count = 0; }
void TimerZone_Reset(void) { g_timerzone_count = 0; }
